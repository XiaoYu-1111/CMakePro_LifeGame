#include "Cuda_Check.cuh"
#include <device_launch_parameters.h>
#include <iostream>
#include <string.h>
#include <time.h> // 引入 time 库获取系统时间

#include <thrust/reduce.h>
#include <thrust/device_ptr.h>
#include <thrust/execution_policy.h>

// ============================================================================
// 1. 基础硬件测试与设备信息获取 (保持不变)
// ============================================================================

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

// ============================================================================
// 2. 无状态哈希随机数引擎 (高带宽、零显存占用)
// ============================================================================

// 高效 PCG 32 哈希，可在 GPU 寄存器内直接高随机度打散输入 seed
__device__ float GetRandomFloatStateless(unsigned int index, unsigned int seed) {
    unsigned int state = index * 747796405U + 2891336453U + seed;
    unsigned int word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    unsigned int result = (word >> 22u) ^ word;
    return (float)result / 4294967295.0f; // 映射至 [0.0, 1.0]
}

// 保持为空函数，无缝兼容现有 C++ 端接口调用
extern "C" void InitCudaLife(int w, int h) {
    // 采用无状态架构后，此处不再需要任何分配和初始化操作
}

// 核心：无状态随机初始化核函数
__global__ void kSeedKernel(uint8_t* world, unsigned int seed, float density, int w, int h) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < w * h) {
        float r = GetRandomFloatStateless(idx, seed);
        world[idx] = (r < density) ? 1 : 0;
    }
}

extern "C" void SeedCudaLife(uint8_t* d_world, int w, int h, float density) {
    int n = w * h;
    unsigned int seed = (unsigned int)time(NULL); // 采用系统时间作为熵源

    int threads = 256;
    int blocks = (n + threads - 1) / threads;

    kSeedKernel << <blocks, threads >> > (d_world, seed, density, w, h);
    cudaDeviceSynchronize(); // 确保写入就绪
}

// ============================================================================
// 3. 核心演化与物理更新核函数 (已应用 GPU 汇编级优化指令)
// ============================================================================

// 核心：逻辑更新核函数 (采用分支条件选择 selp 替代 8 次模运算 %)
__global__ void kLifeUpdate(const uint8_t* current, uint8_t* next, float* heatMap, int w, int h, float dt,
    bool paused, float decay, int b_mask, int s_mask) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= w || y >= h) return;
    int idx = y * w + x;

    if (!paused) {
        int neighbors = 0;

        // 避开昂贵的 `%` 运算符，利用极其高效的 GPU 分支做 Torus 边界包裹
        for (int dy = -1; dy <= 1; dy++) {
            int ny = y + dy;
            if (ny < 0) ny = h - 1;
            else if (ny >= h) ny = 0;

            int rowOffset = ny * w;

            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;

                int nx = x + dx;
                if (nx < 0) nx = w - 1;
                else if (nx >= w) nx = 0;

                neighbors += current[rowOffset + nx];
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
        hVal *= decay;
        if (hVal < 0.005f) hVal = 0.0f; // 过渡截断，保证平滑消逝
    }
    heatMap[idx] = hVal;
}

extern "C" void UpdateLifeCuda(uint8_t* d_current, uint8_t* d_next, float* d_heatMap, int w, int h, float deltaTime, bool paused, float decay, int b_mask, int s_mask) {
    dim3 block(16, 16);
    dim3 grid((w + 15) / 16, (h + 15) / 16);
    kLifeUpdate << <grid, block >> > (d_current, d_next, d_heatMap, w, h, deltaTime, paused, decay, b_mask, s_mask);
}

// 鼠标交互绘制核函数 (避开 powf 和 sqrtf 指令，使用距离平方进行比较)
__global__ void kMousePaint(uint8_t* world, float* heatMap, int w, int h, int mx, int my, int radius, bool erase) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= w || y >= h) return;

    // 距离平方运算 (避免了开平方和乘方运算，极大减轻 SFU 单元负载)
    int dx = x - mx;
    int dy = y - my;
    if (dx * dx + dy * dy < radius * radius) {
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

// ============================================================================
// 4. 人口数量规约统计 (Thrust 并行加速)
// ============================================================================
extern "C" int GetPopulationCuda(uint8_t* d_world, int w, int h) {
    // 将原始显存指针包装为 thrust 迭代器
    thrust::device_ptr<uint8_t> ptr(d_world);
    try {
        // 利用 Thrust 在 GPU 上直接执行规约求和 (1 代表活，0 代表死，求和即为总人口)
        return thrust::reduce(thrust::device, ptr, ptr + (w * h), (int)0, thrust::plus<int>());
    }
    catch (...) {
        return 0; // 异常安全屏障
    }
}