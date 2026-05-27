这是一份为你的 CUDA 细胞自动机（康威生命游戏扩展版）项目定制的 `README.md` 文件。它包含了项目简介、技术特点、核心算法说明以及如何使用。

---

# CUDA-Life-Simulation

基于 NVIDIA CUDA 加速的高性能二维细胞自动机（Cellular Automata）模拟引擎。本项目不仅实现了经典的康威生命游戏（Conway's Game of Life），还引入了动态热力图（Heatmap）衰减算法和高度可配置的生存/繁殖规则。

## 🚀 核心特性

- **GPU 全并行计算**：利用 CUDA 核函数对数以百万计的细胞进行实时演化。
- **动态规则引擎**：通过位掩码（Bitmask）支持任意 `B(irth)/S(urvive)` 规则（如经典的 B3/S23）。
- **热力图系统**：内置细胞活跃度衰减算法，能够产生华丽的视觉拖尾效果。
- **实时交互**：支持基于 GPU 的鼠标笔刷绘制和擦除功能。
- **高性能规约**：使用 `Thrust` 库实现极速的人口普查（Population Count）。
- **设备感知**：自动识别 GPU 架构并计算最佳 CUDA 核心利用率。

## 🛠️ 技术细节

### 1. 核心算子：并行卷积与逻辑更新
`kLifeUpdate` 核函数采用并行卷积思想，每个线程处理一个像素，通过环面拓扑（Toroidal Topology）处理边界，实现无限循环世界。

```cpp
// 核心逻辑：位运算检查规则
if (alive) {
    next[idx] = (s_mask & (1 << neighbors)) ? 1 : 0;
} else {
    next[idx] = (b_mask & (1 << neighbors)) ? 1 : 0;
}
```

### 2. 热力图衰减算法
通过 `decay` 参数控制历史轨迹的消失速度，公式如下：
- 若细胞存活：`Heat = 1.0`
- 若细胞死亡：`Heat = Heat * decay`

### 3. 随机初始化
集成 `curand` 库，在显存中直接初始化随机状态（`curandState`），避免了主机与设备间高频的数据拷贝。

## 📂 文件结构说明

| 文件 | 说明 |
| :--- | :--- |
| `kernel.cu` | 核心 CUDA 实现，包含所有演化、初始化、交互核函数 |
| `Cuda_Check.cuh` | CUDA 错误检查辅助工具 |
| `GpuInfo` 结构体 | 导出 GPU 硬件信息（计算能力、核心数、显存等） |
| `extern "C"` 接口 | 提供给外部（如 C# WPF/Unity 或 C++ GUI）调用的标准接口 |

## 🕹️ 导出 API 接口

项目采用了 `extern "C"` 导出，方便集成到各种 UI 框架中：

- `GetCudaDeviceInfo()`: 获取当前显卡详细硬件参数。
- `InitCudaLife(w, h)`: 初始化随机数种子和显存分配。
- `SeedCudaLife(...)`: 按指定密度随机撒种。
- `UpdateLifeCuda(...)`: 执行一帧演化，支持实时调整规则和衰减系数。
- `MousePaintCuda(...)`: 响应鼠标输入，实时修改世界状态。
- `GetPopulationCuda(...)`: 获取当前存活细胞总数。

## 性能对比：GPU 暴力并行 vs. Hashlife

本项目采用的是 **GPU 暴力并行方案**，其与著名的 **Hashlife** 算法对比：

| 维度 | 本方案 (CUDA) | Hashlife |
| :--- | :--- | :--- |
| **擅长场景** | 随机、混乱、高动态演化 | 具有高度重复结构的宏观模式 |
| **实时性** | 极高，适合 60FPS 丝滑渲染 | 较低，通常用于“跳跃式”模拟 |
| **视觉效果** | 支持平滑的热力图拖尾 | 难以实现逐帧的平滑特效 |
| **实现难度** | 中等，易于维护和修改规则 | 极高，涉及复杂四叉树和哈希表 |

## ⚙️ 环境要求

- **操作系统**: Windows 10/11 或 Linux
- **显卡**: 支持 CUDA 的 NVIDIA GPU (建议 Compute Capability 7.0+)
- **工具链**: CUDA Toolkit 11.0+, MSVC 2019+

---

**Tip**: 在使用 `UpdateLifeCuda` 时，建议将 `decay` 设置在 `0.90` 到 `0.98` 之间，以获得最佳的视觉流星拖尾效果！
