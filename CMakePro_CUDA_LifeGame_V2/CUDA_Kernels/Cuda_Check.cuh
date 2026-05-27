#pragma once
#include <cuda_runtime.h>

// 使用 C 兼容的结构体
struct GpuInfo {
    char name[256];
    int computeCapabilityMajor;
    int computeCapabilityMinor;
    size_t totalMem;
    int multiProcessorCount; // 新增：SM 数量
    int cudaCores;           // 新增：计算出的核心数
    int clockRate;           // 新增：时钟频率
    bool success;
};

#ifdef __cplusplus
extern "C" {
#endif

    // 检查 GPU 硬件信息
    GpuInfo GetCudaDeviceInfo();

    // 运行一个简单的加法核函数测试
    bool RunCudaTest(float* h_a, float* h_b, float* h_c, int n);

#ifdef __cplusplus
}
#endif

//生命游戏模块函数
#ifdef __cplusplus
extern "C" {
#endif
    // 传递指针版本
    void InitCudaLife(int w, int h); // 初始化随机数状态
    void SeedCudaLife(uint8_t* d_world, int w, int h, float density);
    // 增加 decay 参数
    void UpdateLifeCuda(uint8_t* d_current, uint8_t* d_next, float* d_heatMap,
        int w, int h, float deltaTime, bool paused, float decay, int b_mask, int s_mask);
    void MousePaintCuda(uint8_t* d_world, float* d_heat, int w, int h, int mx, int my, int radius, bool erase);
    int GetPopulationCuda(uint8_t* d_world, int w, int h);
#ifdef __cplusplus
}
#endif