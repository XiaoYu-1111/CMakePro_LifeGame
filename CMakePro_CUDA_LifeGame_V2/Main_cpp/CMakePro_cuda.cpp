/**
 * @file    main.cpp
 * @brief   程序主入口：采用“Intro Screen + Simulation”双模式架构
 */

 // =================================================================================
 // 1. C/C++ 标准库 (Standard Libraries)
 // =================================================================================
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <cmath>
#include <algorithm>
#include <complex>
#include <random>
#include <ctime>
#include <filesystem>     // C++17 跨平台路径处理

// =================================================================================
// 2. 图形基础库 (Graphics API & Window Management)
// =================================================================================
// 注意：GLEW 必须在 GLFW 之前包含，以确保正确的 OpenGL 函数指针加载
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// =================================================================================
// 3. 用户界面库 (User Interface Libraries)
// =================================================================================
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"       // ImGui 绘图扩展

// =================================================================================
// 4. 图像处理工具 (Image Utilities - stb)
// =================================================================================
// 警告：IMPLEMENTATION 宏只能在一个 .cpp 文件中定义，否则会导致符号重定义错误
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

// =================================================================================
// 5. 本项目自定义头文件 (Project Specific Headers)
// =================================================================================
#include "CMakePro_cuda.h" // 包含启动界面逻辑、系统结构体及核心计算单元
 // =================================================================================
 // 主函数
 // =================================================================================

int main() {

    std::cout << "========= CUDA 硬件环境测试 =========" << std::endl;
    // 1. 测试获取 GPU 信息
    GpuInfo info = GetCudaDeviceInfo();
    if (info.success) {
        std::cout << "[成功] 找到 GPU 设备:" << std::endl;
        std::cout << "   - 设备名称: " << info.name << std::endl;
        std::cout << "   - 计算能力: " << info.computeCapabilityMajor << "." << info.computeCapabilityMinor << std::endl;
        std::cout << "   - 显存总量: " << (info.totalMem / 1024 / 1024) << " MB" << std::endl;
    }
    else {
        std::cerr << "[失败] 未找到 CUDA 设备或驱动程序异常！" << std::endl;
        return -1;
    }

    // 1. 系统与配置对象初始化
    SimState state;         // 状态管理
    SimConfig config;       // 程序配置
    GLHandles gl;           // OpenGL 资源句柄
    AsyncLoader g_loader;   // 后台加载器

    // ============================================================
    // 1.1 GLFW 与 OpenGL 窗口环境搭建
    // ============================================================
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 设置 OpenGL 4.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(config.winWidth, config.winHeight, "LifeGame_GPU 1.1.1", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &config.winWidth, &config.winHeight);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, &gl); // 将 GL 句柄绑定到窗口以便后续回调

    // 加载窗口图标
    GLFWimage images[1];
    std::string image_icon = "../resources_LifeGame_V2/images/icon_ls.png";
    images[0].pixels = stbi_load(image_icon.c_str(), &images[0].width, &images[0].height, 0, 4);
    if (images[0].pixels) {
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);
    }

    // 初始化 GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    InitQuad(gl);//
    // 加载渲染着色器 (可视化)
    gl.renderProg = glCreateProgram();
    std::string vsSrc = loadShaderFromFile("../resources_LifeGame_V2/shader/vs_code.vert");
    std::string fsSrc = loadShaderFromFile("../resources_LifeGame_V2/shader/fs_code.frag");
    GLuint vs = createShader(vsSrc, GL_VERTEX_SHADER);
    GLuint fs = createShader(fsSrc, GL_FRAGMENT_SHADER);
    glAttachShader(gl.renderProg, vs);
    glAttachShader(gl.renderProg, fs);
    glLinkProgram(gl.renderProg);


    // 在 gl.renderProg 链接之后
    glGenTextures(1, &gl.lifeTex);
    glBindTexture(GL_TEXTURE_2D, gl.lifeTex);
    // 我们使用 GL_R32F 存储热力值（0.0 ~ 1.0）
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 1920, 1080, 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 注册纹理到 CUDA
    cudaGraphicsGLRegisterImage(&gl.cudaRes, gl.lifeTex, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsWriteDiscard);

    // 初始化 CUDA 内部缓冲区
    // --- [初始化部分] ---
    InitCudaLife(1920, 1080); // 初始化随机数生成器
    cudaMalloc(&gl.d_current, 1920 * 1080);
    cudaMalloc(&gl.d_next, 1920 * 1080);
    cudaMalloc(&gl.d_heatData, 1920 * 1080 * sizeof(float));

    // 必须：给初始数据！
    SeedCudaLife(gl.d_current, 1920, 1080, 0.3f);

    // 初始化 ImGui 与 ImPlot
    Init_Imgui(window);
    ImPlot::CreateContext();

    // ============================================================
    // 1.2 初始状态设定
    // ============================================================
    state.isIntroMode = true; // 强制从启动页开始
    // 在循环开始前，先根据初始状态设置一次
    glfwSwapInterval(state.vsyncEnabled ? 1 : 0);
    // ============================================================
    // 2. 主循环 (Main Loop)
    // ============================================================
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // 1. 首先清除整个屏幕（必须在所有渲染之前）
        glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // VSync 动态切换逻辑 ---
        static bool lastVsync = state.vsyncEnabled;
        if (state.vsyncEnabled != lastVsync) {
            glfwSwapInterval(state.vsyncEnabled ? 1 : 0);
            lastVsync = state.vsyncEnabled;
        }

        // 帧信息获取
        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);
        CheckIdleStatus(window, state); // 检测用户是否空闲

        // 2. ImGui 准备新帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- 键盘切换逻辑 ---
        ProcessInput(window, config, state, gl);//处理输入 (Input Processing)
        // 使用新的分发器渲染

        RenderApp(state.currentScreen, state, info, winW, winH,  gl, g_loader);
        state.isIntroMode = IsIntroScreen(state.currentScreen); // 同步状态

        // --- 渲染提交 ---
        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 处理多视口 (如果 ImGui 开启了多窗口支持)
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_context);
        }
        glfwSwapBuffers(window);
    }

    // ============================================================
    // 3. 资源清理 (Cleanup)
    // ============================================================
    cleanup(gl); // 确保实现此函数以释放纹理/Shader资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}