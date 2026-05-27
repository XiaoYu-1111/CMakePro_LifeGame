#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include "imgui.h"

// ===================================================================
// 1. 导航与界面枚举
// ===================================================================
enum class AppScreen {
    Intro0 = 0,     // 启动欢迎页 (Logo/版权)
    Intro1,         // 引导页1
    Intro2,         // 引导页2
    LifeGame,       // 生命游戏cpu
    LifeGame2,       // 生命游戏gpu
    HamStation,
    CudaDiagno,
    Count
};

// ===================================================================
// 2. 模拟与应用程序状态
// ===================================================================
struct SimState {
    AppScreen currentScreen = AppScreen::Intro0; // 当前激活屏幕
    bool isIntroMode = true;        // 是否处于引导模式
    bool running = false;           // 模拟运行状态
    bool vsyncEnabled = true;       // 垂直同步开关

    // --- UI 显示控制 ---
    bool showControls = true;       // 控制面板显示
    bool showTelemetry = true;      // 遥测面板显示
    bool showBgGrid = true;         // 背景装饰网格显示

    bool autoZoom = true;

    // --- 分型渲染逻辑 ---
    double fractalZoom = 2.0;       // 缩放深度 (使用double保证深入精度的稳定)
    int styleMode = 0; // 0: 有机组织, 1: 绿叶脉络

    // --- 用户空闲检测 ---
    float lastInputTime = 0.0;      // 上次输入时间点
    float idleTimeout = 60.0;       // 空闲超时阈值 (秒)
    double lastMouseX = 0.0;        // 鼠标X轴缓存
    double lastMouseY = 0.0;        // 鼠标Y轴缓存
};

// ===================================================================
// 3. 全局配置参数
// ===================================================================
struct SimConfig {
    int simWidth = 1024;           // 模拟计算宽度
    int simHeight = 512;            // 模拟计算高度
    int winWidth = 1000;           // 初始窗口宽度
    int winHeight = 1000;           // 初始窗口高度
    ImVec4 back_color = ImVec4(0.12f, 0.13f, 0.18f, 1.0f); // 界面清除色
};

// RLE 结构体，用于存储解析后的图案
struct RlePattern {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> data; // 行主序存储 (0 为死, 1 为活)
};

// ===================================================================
// 4. OpenGL/GPU 资源句柄与 Uniform 缓存
// ===================================================================
#include <cuda_runtime.h>
#include <cuda_gl_interop.h> // 必须包含这个头文件以支持互操作

struct GLHandles {
    // --- 1. 渲染程序与基础几何 ---
    GLuint renderProg = 0;           // 片段着色器程序 (渲染全屏纹理)
    GLuint quadVAO = 0, quadVBO = 0; // 全屏矩形的顶点数据

    // --- 2. OpenGL 纹理 (生命游戏数据中心) ---
    GLuint lifeTex = 0;              // 映射到显存的纹理句柄 (存储热力值)

    // --- 3. CUDA-OpenGL 互操作句柄 ---
    // 注意：cudaRes 必须是指针类型 (cudaGraphicsResource_t)
    struct cudaGraphicsResource* cudaRes = nullptr;

    // 映射后的 CUDA 数组，用于将 CUDA 计算结果拷贝到 OpenGL 纹理
    cudaArray_t cudaArray = nullptr;

    // --- 4. CUDA 专用显存缓冲区 (Device Pointers) ---
    // 为了效率，逻辑计算使用原始显存指针，计算后再拷贝到上面的纹理
    uint8_t* d_current = nullptr;    // 当前代细胞状态 (0 或 1)
    uint8_t* d_next = nullptr;       // 下一代细胞状态
    float* d_heatData = nullptr;   // 显存中的热力值数组 (0.0f - 1.0f)

    int simW = 1920;
    int simH = 1080;

};

// ===================================================================
// 5. 异步加载与模型数据结构
// ===================================================================
struct ModelLoadResult {
    bool success = false;
    int width = 0, height = 0;
    std::vector<float> data;
};

struct AsyncLoader {
    std::thread worker;                   // 后台加载线程
    std::atomic<bool> isLoading{ false }; // 是否加载中
    std::atomic<bool> isDataReady{ false };// 数据是否准备就绪
    std::atomic<float> progress{ 0.0f };  // 加载进度 (0.0-1.0)
    std::string statusMsg = "IDLE";       // 状态描述信息
    ModelLoadResult tempResult;           // 临时结果缓存
    std::mutex dataMutex;                 // 线程保护锁
};

// ===================================================================
// 6. 实体与基础结构
// ===================================================================
struct Particle {
    ImVec2 pos;  // 位置
    ImVec2 vel;  // 速度
    float type;  // 粒子类型 (0=纵波, 1=横波/渲染着色)
};

// ===================================================================
// 7. Shader 实用工具函数 (实现)
// ===================================================================

/**
 * @brief 从文件读取着色器源码
 */
inline std::string loadShaderFromFile(const std::string& filePath) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        return shaderStream.str();
    }
    catch (const std::exception& e) {
        std::cerr << "CORE::SHADER::FILE_NOT_FOUND: " << filePath << " | " << e.what() << std::endl;
        return "";
    }
}

/**
 * @brief 编译并创建单个着色器对象
 */
inline GLuint createShader(const std::string& source, GLenum type) {
    if (source.empty()) return 0;
    const char* src = source.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "CORE::SHADER::COMPILATION_ERROR ("
            << (type == GL_VERTEX_SHADER ? "VERT" : (type == GL_FRAGMENT_SHADER ? "FRAG" : "COMP"))
            << ")\n" << infoLog << std::endl;
        return 0;
    }
    return shader;
}

inline void InitQuad(GLHandles& gl) {
    // 定义 4 个顶点：位置(x,y,z) + 纹理坐标(u,v)
    float vertices[] = {
        // 位置              // 纹理坐标
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // 左上
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // 左下
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // 右下

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,  // 左上
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // 右下
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f   // 右上
    };

    glGenVertexArrays(1, &gl.quadVAO);
    glGenBuffers(1, &gl.quadVBO);

    glBindVertexArray(gl.quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gl.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // 位置属性 (Location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    // 纹理坐标属性 (Location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

// 辅助函数：解析头部格式，高度兼容空格差异
inline bool ParseRleHeader(const std::string& headerLine, int& outW, int& outH) {
    std::string line = headerLine;
    // 移除所有空格
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

    size_t xPos = line.find("x=");
    size_t yPos = line.find("y=");
    if (xPos == std::string::npos || yPos == std::string::npos) return false;

    // 截取 x 值 (到逗号结束)
    size_t commaAfterX = line.find(',', xPos);
    std::string xStr = line.substr(xPos + 2, commaAfterX - (xPos + 2));

    // 截取 y 值 (到下一个逗号或结尾结束)
    size_t commaAfterY = line.find(',', yPos);
    std::string yStr = (commaAfterY == std::string::npos) ?
        line.substr(yPos + 2) :
        line.substr(yPos + 2, commaAfterY - (yPos + 2));

    try {
        outW = std::stoi(xStr);
        outH = std::stoi(yStr);
        return true;
    }
    catch (...) {
        return false;
    }
}

// 主解析器：解析外部 RLE 文件
inline RlePattern ParseRleFile(const std::string& filepath) {
    RlePattern pattern;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "RLE_PARSER::ERROR: Failed to open file: " << filepath << std::endl;
        return pattern;
    }

    std::string line;
    std::string dataStr = "";
    bool headerFound = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue; // 跳过注释行

        // 解析头部行
        if (!headerFound && line.find('x') != std::string::npos && line.find('y') != std::string::npos) {
            if (ParseRleHeader(line, pattern.width, pattern.height)) {
                headerFound = true;
                // 预分配为全死细胞 (0)
                pattern.data.assign(pattern.width * pattern.height, 0);
            }
            continue;
        }

        // 收集之后的所有数据行
        if (headerFound) {
            dataStr += line;
        }
    }

    if (!headerFound) {
        std::cerr << "RLE_PARSER::ERROR: No valid header found in " << filepath << std::endl;
        return pattern;
    }

    // 解码游程数据
    int curX = 0;
    int curY = 0;
    int run = 0;

    for (size_t i = 0; i < dataStr.size(); ++i) {
        char c = dataStr[i];
        if (std::isdigit(c)) {
            run = run * 10 + (c - '0'); // 累积多位数
        }
        else if (c == 'b' || c == 'o' || c == '$' || c == '!') {
            int actualRun = (run == 0) ? 1 : run;
            run = 0; // 重置计数器

            if (c == '!') {
                break; // 结束标记
            }
            else if (c == 'b') {
                curX += actualRun; // 死细胞直接位移
            }
            else if (c == 'o') {
                // 活细胞填充
                for (int r = 0; r < actualRun; ++r) {
                    if (curX < pattern.width && curY < pattern.height) {
                        pattern.data[curY * pattern.width + curX] = 1;
                    }
                    curX++;
                }
            }
            else if (c == '$') {
                curY += actualRun; // 下移多行
                curX = 0;          // 回到行首
            }
        }
        // 自动忽略由于换行带来的空格、回车等空白字符
    }

    return pattern;
}

#include <cuda_runtime.h>

// 将 RlePattern 直接载入 GPU 模拟区的指定偏移位置 (offsetX, offsetY)
inline void LoadRleToGpu(GLHandles& gl, const RlePattern& pattern, int offsetX, int offsetY) {
    if (pattern.data.empty() || pattern.width <= 0 || pattern.height <= 0) {
        std::cerr << "RLE_GPU_LOADER::ERROR: Empty pattern structure." << std::endl;
        return;
    }

    // 1. 边界裁剪逻辑
    int copyW = pattern.width;
    int copyH = pattern.height;

    if (offsetX + copyW > gl.simW) {
        copyW = gl.simW - offsetX;
        std::cout << "RLE_GPU_LOADER::WARNING: Pattern horizontally truncated to fit simulation grid." << std::endl;
    }
    if (offsetY + copyH > gl.simH) {
        copyH = gl.simH - offsetY;
        std::cout << "RLE_GPU_LOADER::WARNING: Pattern vertically truncated to fit simulation grid." << std::endl;
    }

    if (copyW <= 0 || copyH <= 0) {
        std::cerr << "RLE_GPU_LOADER::ERROR: Invalid offset, pattern is entirely out of grid bounds." << std::endl;
        return;
    }

    // 2. 计算目标指针偏移位置
    // 在 1D 线性显存中找到 (offsetX, offsetY) 的起始字节地址
    uint8_t* d_dest = gl.d_current + (size_t)offsetY * gl.simW + offsetX;

    // 主机端源数据指针
    const uint8_t* h_src = pattern.data.data();

    // 3. 执行高效 2D 显存拷贝 (Host to Device)
    size_t dpitch = gl.simW * sizeof(uint8_t);      // 目标网格跨度（字节）
    size_t spitch = pattern.width * sizeof(uint8_t); // 源图案跨度（字节）
    size_t widthInBytes = copyW * sizeof(uint8_t);   // 实际拷贝矩阵的单行宽度

    cudaError_t err = cudaMemcpy2D(
        d_dest,             // 显存目的指针偏移
        dpitch,             // 目的指针步长 (Pitch)
        h_src,              // 主机内存源指针
        spitch,             // 源指针步长 (Pitch)
        widthInBytes,       // 拷贝的矩形宽度
        copyH,              // 拷贝的矩形高度
        cudaMemcpyHostToDevice
    );

    if (err != cudaSuccess) {
        std::cerr << "RLE_GPU_LOADER::CUDA_ERROR: " << cudaGetErrorString(err) << std::endl;
        return;
    }

    // 4. 同步当前缓冲至下一帧，防止演化覆盖导致闪烁或撕裂
    cudaMemcpy(gl.d_next, gl.d_current, (size_t)gl.simW * gl.simH, cudaMemcpyDeviceToDevice);

    // 初始化热力图对应区域
    // （可选操作：如果想让新加入的飞船尾迹直接亮起，可以把 heatMap 对应位置设为 1.0f）
    float* d_heatDest = gl.d_heatData + (size_t)offsetY * gl.simW + offsetX;
    std::vector<float> h_heatOnes(pattern.width * pattern.height, 1.0f);
    cudaMemcpy2D(
        d_heatDest,
        gl.simW * sizeof(float),
        h_heatOnes.data(),
        pattern.width * sizeof(float),
        copyW * sizeof(float),
        copyH,
        cudaMemcpyHostToDevice
    );

    std::cout << "RLE_GPU_LOADER::SUCCESS: Pattern size [" << pattern.width << "x" << pattern.height
        << "] successfully loaded at (" << offsetX << ", " << offsetY << ")" << std::endl;
}

/**
 * @brief 极速裁剪 RLE 解析器：只解析并加载指定区域
 * @param startX 裁剪区域的左上角 X
 * @param startY 裁剪区域的左上角 Y
 * @param cropW  裁剪宽度
 * @param cropH  裁剪高度
 */
inline RlePattern ParseRleFileCropped(const std::string& filepath, int startX, int startY, int cropW, int cropH) {
    RlePattern pattern;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "RLE_CROP_PARSER::ERROR: Failed to open file: " << filepath << std::endl;
        return pattern;
    }

    std::string line;
    std::string dataStr = "";
    bool headerFound = false;
    int originalW = 0, originalH = 0;

    // 1. 读取头部以获取原图真实大小并进行边界安全检查
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (!headerFound && line.find('x') != std::string::npos && line.find('y') != std::string::npos) {
            if (ParseRleHeader(line, originalW, originalH)) {
                headerFound = true;

                // 限制裁剪范围不能超出原图
                if (startX >= originalW || startY >= originalH) {
                    std::cerr << "RLE_CROP_PARSER::ERROR: Crop origin out of bounds!" << std::endl;
                    return pattern;
                }
                if (startX + cropW > originalW) cropW = originalW - startX;
                if (startY + cropH > originalH) cropH = originalH - startY;

                pattern.width = cropW;
                pattern.height = cropH;
                pattern.data.assign(cropW * cropH, 0); // 仅为裁剪后的大小分配内存
            }
            continue;
        }

        if (headerFound) {
            dataStr += line;
        }
    }

    if (!headerFound) return pattern;

    // 2. 解解数据并动态裁剪
    int curX = 0;
    int curY = 0;
    int run = 0;

    for (size_t i = 0; i < dataStr.size(); ++i) {
        char c = dataStr[i];
        if (std::isdigit(c)) {
            run = run * 10 + (c - '0');
        }
        else if (c == 'b' || c == 'o' || c == '$' || c == '!') {
            int actualRun = (run == 0) ? 1 : run;
            run = 0;

            if (c == '!') break;

            if (c == 'b') {
                curX += actualRun;
            }
            else if (c == 'o') {
                for (int r = 0; r < actualRun; ++r) {
                    // 仅当细胞落在裁剪矩形内时，记录到 pattern 中
                    if (curX >= startX && curX < startX + cropW &&
                        curY >= startY && curY < startY + cropH) {
                        int relX = curX - startX;
                        int relY = curY - startY;
                        pattern.data[relY * cropW + relX] = 1;
                    }
                    curX++;
                }
            }
            else if (c == '$') {
                curY += actualRun;
                curX = 0;

                // 【核心优化点】
                // 如果当前解析行数已经超过了裁剪区域的最大高度 (startY + cropH)，
                // 因为 RLE 是从上到下按行存储的，后续行肯定都不在裁剪范围内，可以直接安全退出！
                if (curY >= startY + cropH) {
                    break;
                }
            }
        }
    }

    return pattern;
}