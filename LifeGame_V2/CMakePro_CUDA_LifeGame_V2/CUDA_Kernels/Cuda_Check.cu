#include "Cuda_Check.cuh"
#include <device_launch_parameters.h>
#include <iostream>
#include <string.h>

#include <thrust/reduce.h>
#include <thrust/device_ptr.h>

#include <thrust/execution_policy.h>

// 核函数
__global__ void vectorAddKernel(const float* a, const float* b, float* c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) c[i] = a[i] + b[i];
}

// 辅助函数：根据架构计算核心数
int _ConvertSMVerToCores(int major, int minor) {
    // 定义不同架构下每个 SM 包含的核心数
    switch (major) {
    case 7: return (minor == 0 || minor == 5) ? 64 : 64;  // Volta, Turing
    case 8: return (minor == 0) ? 64 : 128;               // Ampere (8.0=64, 8.6/8.9=128)
    case 9: return 128;                                   // Hopper
    default: return 128;
    }
}

extern "C" GpuInfo GetCudaDeviceInfo() {
    GpuInfo info;
    memset(&info, 0, sizeof(info));
    int deviceCount = 0;
    if (cudaGetDeviceCount(&deviceCount) != cudaSuccess || deviceCount == 0) return info;

    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    strncpy_s(info.name, prop.name, sizeof(info.name));
    info.computeCapabilityMajor = prop.major;
    info.computeCapabilityMinor = prop.minor;
    info.totalMem = prop.totalGlobalMem;
    info.multiProcessorCount = prop.multiProcessorCount;
    info.clockRate = prop.clockRate;
    // 计算核心数：SMs * CoresPerSM
    info.cudaCores = prop.multiProcessorCount * _ConvertSMVerToCores(prop.major, prop.minor);
    info.success = true;
    return info;
}


extern "C" bool RunCudaTest(float* h_a, float* h_b, float* h_c, int n) {
    float* d_a = nullptr, * d_b = nullptr, * d_c = nullptr;
    size_t size = n * sizeof(float);

    // 1. 分配显存
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    // 2. 拷贝数据到显存
    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // 3. 启动核函数 (注意：不要有空格)
    // 如果 n > 1024，建议使用多 block
    int threads = (n > 1024) ? 1024 : n;
    int blocks = (n + threads - 1) / threads;
    vectorAddKernel << <blocks, threads >> > (d_a, d_b, d_c, n);

    // 4. 同步并检查错误
    cudaDeviceSynchronize();
    cudaError_t err = cudaGetLastError();

    // 5. 拷贝结果回内存
    if (err == cudaSuccess) {
        cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);
    }

    // 6. 释放资源
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return err == cudaSuccess;
}


#include <cuda_runtime.h>
#include <curand_kernel.h>


static curandState* d_randStates = nullptr;

// 内部使用的随机数状态，只需初始化一次
static curandState* d_states = nullptr;


// --- [核心修复] 定义标准的核函数代替 Lambda ---
__global__ void kInitRand(curandState* states, unsigned long seed, int w, int h) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < w * h) {
        // 每个线程初始化自己的随机数状态
        curand_init(seed, idx, 0, &states[idx]);
    }
}

extern "C" void InitCudaLife(int w, int h) {
    if (d_states) cudaFree(d_states); // 如果已存在则释放
    cudaMalloc(&d_states, w * h * sizeof(curandState));

    int threads = 256;
    int blocks = (w * h + threads - 1) / threads;
    kInitRand << <blocks, threads >> > (d_states, (unsigned long)time(0), w, h);
    cudaDeviceSynchronize();
}

// 核心：随机初始化核函数
__global__ void kSeedKernel(uint8_t* world, curandState* states, float density, int w, int h) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < w * h) {
        float r = curand_uniform(&states[idx]);
        world[idx] = (r < density) ? 1 : 0;
    }
}

extern "C" void SeedCudaLife(uint8_t* d_world, int w, int h, float density) {
    int n = w * h;
    kSeedKernel << <(n + 255) / 256, 256 >> > (d_world, d_states, density, w, h);
    cudaDeviceSynchronize(); // 确保写完
}

// 核心：逻辑更新核函数///并行卷积算子。
__global__ void kLifeUpdate(const uint8_t* current, uint8_t* next, float* heatMap, int w, int h, float dt,
    bool paused, float decay, int b_mask, int s_mask) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= w || y >= h) return;
    int idx = y * w + x;

    if (!paused) {
        int neighbors = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                neighbors += current[((y + dy + h) % h) * w + ((x + dx + w) % w)];
            }
        }
        uint8_t alive = current[idx];
        // 使用位运算检查规则：判断 neighbors 对应的位是否在掩码中
        if (alive) {
            next[idx] = (s_mask & (1 << neighbors)) ? 1 : 0;
        }
        else {
            next[idx] = (b_mask & (1 << neighbors)) ? 1 : 0;
        }
    }
    // 热力图计算
    float hVal = heatMap[idx];
    if (next[idx] == 1) {
        hVal = 1.0f;
    }
    else {
        // 使用传入的参数 decay。建议范围：0.5 (几乎无轨迹) 到 0.99 (长轨迹)
        hVal *= decay;
        if (hVal < 0.005f) hVal = 0.0f; // 稍微调低截断值，让过渡更平滑
    }
    heatMap[idx] = hVal;
}

extern "C" void UpdateLifeCuda(uint8_t* d_current, uint8_t* d_next, float* d_heatMap, int w, int h, float deltaTime, bool paused, float decay, int b_mask, int s_mask) {
    dim3 block(16, 16);
    dim3 grid((w + 15) / 16, (h + 15) / 16);
    kLifeUpdate << <grid, block >> > (d_current, d_next, d_heatMap, w, h, deltaTime, paused, decay, b_mask, s_mask);
}

__global__ void kMousePaint(uint8_t* world, float* heatMap, int w, int h, int mx, int my, int radius, bool erase) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= w || y >= h) return;

    float dist = sqrtf(powf(x - mx, 2) + powf(y - my, 2));
    if (dist < radius) {
        int idx = y * w + x;
        world[idx] = erase ? 0 : 1;
        if (!erase) heatMap[idx] = 1.0f;
    }
}

extern "C" void MousePaintCuda(uint8_t* d_world, float* d_heat, int w, int h, int mx, int my, int radius, bool erase) {
    dim3 block(16, 16);
    dim3 grid((w + 15) / 16, (h + 15) / 16);
    kMousePaint << <grid, block >> > (d_world, d_heat, w, h, mx, my, radius, erase);
}

extern "C" int GetPopulationCuda(uint8_t* d_world, int w, int h) {
    // 将原始显存指针包装为 thrust 迭代器
    thrust::device_ptr<uint8_t> ptr(d_world);
    // 执行规约求和 (1 代表活，0 代表死，求和即为总人口)
    // 使用 thrust::plus<int>() 进行累加
    try {
        return thrust::reduce(thrust::device, ptr, ptr + (w * h), (int)0, thrust::plus<int>());
    }
    catch (...) {
        return 0; // 防止异常
    }
}