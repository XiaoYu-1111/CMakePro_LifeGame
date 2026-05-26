// CMakePro_gui_intro.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include "Common.h"


#include "Cuda_Check.cuh"//cuda计算头文件

constexpr float PI = 3.14159265358979323846f;

void SetupImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // ==========================================================
    // 1. 几何形状 (Geometry) - 模仿 macOS Big Sur+ 的圆润感
    // ==========================================================
    style.WindowRounding = 10.0f; // 窗口大圆角
    style.ChildRounding = 6.0f;
    style.FrameRounding = 6.0f;  // 输入框/按钮圆角
    style.GrabRounding = 12.0f; // 滚动条/滑块手柄变成圆形
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 9.0f;
    style.TabRounding = 6.0f;

    style.WindowBorderSize = 0.0f; // 现代风格去边框
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f; // 弹窗保留细边框以增加立体感

    // 增加间距，让界面更透气（Apple 风格的精髓）
    style.ItemSpacing = ImVec2(8, 6);
    style.WindowPadding = ImVec2(16, 16);
    style.FramePadding = ImVec2(10, 6); // 按钮稍微胖一点

    // ==========================================================
    // 2. 配色方案 (Palette) - 深空灰 + 系统蓝
    // ==========================================================
    ImVec4* colors = style.Colors;

    // --- 核心色调 ---
    // Apple 系统蓝 (System Blue)
    ImVec4 accent = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);
    ImVec4 accentHover = ImVec4(0.12f, 0.56f, 1.00f, 1.00f);
    ImVec4 accentActive = ImVec4(0.00f, 0.36f, 0.85f, 1.00f);

    // 背景色 (类似于 Finder/Xcode 的深色背景)
    ImVec4 bgMain = ImVec4(0.11f, 0.11f, 0.11f, 1.00f); // 极深灰
    ImVec4 bgPanel = ImVec4(ImVec4(0.12f, 0.13f, 0.18f, 1.0f)); // 稍亮的面板灰
    //0.12f, 0.13f, 0.18f, 1.0f
    ImVec4 bgInput = ImVec4(0.22f, 0.22f, 0.22f, 1.00f); // 输入框灰

    // --- 赋值 ---
    colors[ImGuiCol_WindowBg] = bgPanel; // 窗口背景
    colors[ImGuiCol_ChildBg] = bgMain;  // 子窗口背景（更有层次）
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.96f);

    // 边框 (微妙的分割线)
    colors[ImGuiCol_Border] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    // 标题栏
    colors[ImGuiCol_TitleBg] = bgPanel;
    colors[ImGuiCol_TitleBgActive] = bgPanel; // Mac 标题栏不随激活变色，靠红绿灯按钮区分(ImGui做不到)，所以保持一致
    colors[ImGuiCol_TitleBgCollapsed] = bgPanel;

    // 文本
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // SF Pro White
    colors[ImGuiCol_TextDisabled] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);

    // 输入框 & 控件底色
    colors[ImGuiCol_FrameBg] = bgInput;
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    // 按钮 (Mac 风格通常是深灰底，按下变深)
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = accentHover; // 悬停时用蓝色提示
    colors[ImGuiCol_ButtonActive] = accentActive;

    // 勾选框 & 滑块 (使用强调色)
    colors[ImGuiCol_CheckMark] = accent;
    colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f); // Mac 滑块头通常是白/浅灰色的圆点
    colors[ImGuiCol_SliderGrabActive] = accent; // 拖动时变蓝

    // 滚动条 (极简)
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.00f); // 透明轨道
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 0.60f); // 半透明灰条
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

    // 列表头 & 选中项
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.48f, 1.00f, 0.20f); // 淡淡的蓝色背景
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.48f, 1.00f, 0.40f);
    colors[ImGuiCol_HeaderActive] = accent;

    // 分割线
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = accent;

    // 绘图线
    colors[ImGuiCol_PlotLines] = accent;
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = accent;
    colors[ImGuiCol_PlotHistogramHovered] = accentHover;
}

void Init_Imgui(GLFWwindow* window) {
    // 1. 检查版本并创建上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext(); // [新增] 初始化 ImPlot (建议紧跟 ImGui 创建)

    ImGuiIO& io = ImGui::GetIO();

    // 2. 配置 Flags (合并了你代码中重复的部分)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // 允许键盘导航
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // [关键] 启用 Docking (停靠)
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // [关键] 启用 Viewports (多窗口拖出)
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     // 启用手柄 (可选)

    // 3. 字体加载策略
    float baseSize = 14.0f;
    float scaleFactor = 1.5f;
    float fontSize = baseSize * scaleFactor;

    std::string font_path = "../resources_LifeGame_V2/font/consola.ttf";

    // 尝试加载自定义字体
    ImFont* font = io.Fonts->AddFontFromFileTTF(font_path.c_str(), fontSize);

    // [安全检查] 如果加载失败 (比如路径不对)，加载默认字体，防止程序崩溃或无文字
    if (font == nullptr) {
        // 尝试加载 Windows 系统自带的 consolas
        font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", fontSize);
        if (font == nullptr) {
            // 如果还不行，加载 ImGui 内置像素字体
            io.Fonts->AddFontDefault();
            fprintf(stderr, "Warning: Failed to load custom font, using default.\n");
        }
    }

    // 4. 设置样式
    SetupImGuiStyle();

    // [关键] 针对 Viewports 的样式微调
    // 当窗口被拖出主程序时，如果 WindowBg 透明度不是 1.0，可能会看到操作系统桌面，通常很难看
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGuiStyle& style = ImGui::GetStyle();
        //style.WindowRounding = 0.0f; // 浮动窗口通常不需要圆角 (可选)
        style.Colors[ImGuiCol_WindowBg].w = 1.0f; // 强制背景不透明
    }

    // 5. 初始化后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}
// 绘制欢迎/登录界面
// 辅助函数：将 HSV 转为 ImU32 格式
ImU32 GetColorFromHSV(float h, float s, float v, float a) {
    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
    return ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
}
// 主界面0，中转切换
void RenderIntroScreen0(SimState& state, int winW, int winH, bool& isIntroMode, AsyncLoader& loader) {
    const float scale = (float)winW / 1920.0f;
    const float t = (float)ImGui::GetTime();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 screenCenter(winW * 0.5f, winH * 0.5f);

    // =====================================================
    // 1. 背景渲染：钛金深灰色渐变 (参考 IntroScreen2)
    // =====================================================
    // 采用从深钛蓝到炭黑的四角渐变
    drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2((float)winW, (float)winH),
        IM_COL32(35, 42, 52, 255), IM_COL32(35, 42, 52, 255),
        IM_COL32(15, 17, 20, 255), IM_COL32(15, 17, 20, 255));

    // 绘制静态随机噪点，模拟金属质感
    static std::vector<ImVec2> static_noise;
    if (static_noise.empty()) {
        for (int i = 0; i < 1500; i++) static_noise.push_back(ImVec2((float)(rand() % winW), (float)(rand() % winH)));
    }
    for (auto& np : static_noise) drawList->AddRectFilled(np, ImVec2(np.x + 1, np.y + 1), IM_COL32(255, 255, 255, 8));

    // =====================================================
    // 2. 对称钛金网格系统 (参考 IntroScreen2)
    // =====================================================
    float subStep = 30.0f * scale;
    float mainStep = 150.0f * scale;

    // 绘制细微辅助线
    for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += subStep) {
        ImU32 col = IM_COL32(255, 255, 255, 8);
        drawList->AddLine({ screenCenter.x + x, 0 }, { screenCenter.x + x, (float)winH }, col);
        if (x > 0) drawList->AddLine({ screenCenter.x - x, 0 }, { screenCenter.x - x, (float)winH }, col);
    }
    // 绘制主坐标网格与青色交汇点
    for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += mainStep) {
        for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += mainStep) {
            ImU32 dotCol = IM_COL32(0, 255, 255, 40);
            drawList->AddCircleFilled({ screenCenter.x + x, screenCenter.y + y }, 2.5f * scale, dotCol);
            if (x > 0) drawList->AddCircleFilled({ screenCenter.x - x, screenCenter.y + y }, 2.5f * scale, dotCol);
            if (y > 0) drawList->AddCircleFilled({ screenCenter.x + x, screenCenter.y - y }, 2.5f * scale, dotCol);
            if (x > 0 && y > 0) drawList->AddCircleFilled({ screenCenter.x - x, screenCenter.y - y }, 2.5f * scale, dotCol);
        }
    }

    // --- [光纤级] 超高亮度动态发光曲线 (三层叠绘 / 数据流光) ---
    {
        const int lineCount = 9;           // 线条数量，不宜过多以保持精细感
        const int segments = 120;           // 采样点增加，确保曲线极度丝滑
        float baseY = winH * 0.88f;         // 玻璃板底部的光纤束位置

        for (int i = 0; i < lineCount; ++i) {
            // --- 1. 物理轨迹预计算 (存入临时向量以进行多层绘制) ---
            static std::vector<ImVec2> points;
            points.clear();

            float speed = 0.2f + (i * 0.05f);
            float phase = t * speed + i * 0.7f;
            float ampBase = (10.0f + (i % 6) * 12.0f) * scale;

            // 亮度脉动：模拟光纤中信号的波动
            float signalPulse = (sinf(t * 3.0f + i * 0.5f) * 0.15f + 0.85f);

            for (int j = 0; j <= segments; ++j) {
                float ratio = (float)j / (float)segments;
                float x = ratio * winW;

                // 复合波形公式 (主波 + 信号噪声)
                float wave = sinf(ratio * 5.0f + phase) * ampBase;
                wave += cosf(ratio * 12.0f - phase * 0.8f) * (4.0f * scale);

                // 边缘包络线 (两端汇聚，中间发散)
                float envelope = sinf(ratio * 3.14159f);
                float y = baseY + (wave * envelope) + (i * 5.0f * scale);

                points.push_back({ x, y });
            }

            // --- 2. 颜色定义 (高饱和度电光色系) ---
            ImU32 colCore, colBloom, colGlow;
            float baseThickness = (i % 5 == 0) ? 1.0f : 0.6f; // 线条进一步减细

            if (i % 3 == 0) {
                // 类型 A: 冰晶钛白 (冷色调白色，降低权重)
                colCore = IM_COL32(200, 230, 255, (int)(160 * signalPulse)); // 降低到 160
                colBloom = IM_COL32(100, 180, 200, (int)(45 * signalPulse));  // 降低到 45
                colGlow = IM_COL32(80, 150, 180, (int)(15 * signalPulse));   // 降低到 15
            }
            else {
                // 类型 B: 磨砂深钛蓝 (极低饱和度)
                colCore = IM_COL32(120, 150, 180, (int)(130 * signalPulse)); // 降低到 130
                colBloom = IM_COL32(60, 90, 120, (int)(35 * signalPulse));    // 降低到 35
                colGlow = IM_COL32(40, 60, 80, (int)(10 * signalPulse));     // 降低到 10
            }

            // --- 渲染宽度也同步收缩，增加精细感 ---
            // Pass 1: 极弱的背光感 (减少宽度)
            drawList->AddPolyline(points.data(), (int)points.size(), colGlow, 0, 8.0f * scale);
            // Pass 2: 微弱的折射感
            drawList->AddPolyline(points.data(), (int)points.size(), colBloom, 0, 2.5f * scale);
            // Pass 3: 核心纤细线条
            drawList->AddPolyline(points.data(), (int)points.size(), colCore, 0, baseThickness * scale);

            // --- 4. 增强：光纤数据包粒子 (Sparkles) ---
            // 模拟高能数据包在光纤中快速移动
            if (i % 2 == 0) {
                float packetPos = fmodf(t * 0.1f + i * 0.1f, 1.2f); // 移动速度较快
                if (packetPos <= 1.0f) {
                    // 计算粒子在路径上的平滑位置
                    int idx = (int)(packetPos * segments);
                    ImVec2 p = points[idx];
                    float pFade = sinf(packetPos * 3.14159f); // 粒子也在两端淡入淡出

                    // 绘制高亮光斑
                    drawList->AddCircleFilled(p, 3.5f * scale, colCore); // 核心
                    drawList->AddCircleFilled(p, 8.0f * scale, colBloom); // 晕染

                    // 绘制十字星芒 (工业级精密感点缀)
                    float flareSize = 12.0f * scale * pFade;
                    drawList->AddLine({ p.x - flareSize, p.y }, { p.x + flareSize, p.y }, colCore, 1.0f);
                    drawList->AddLine({ p.x, p.y - flareSize }, { p.x, p.y + flareSize }, colCore, 1.0f);
                }
            }
        }
    }

    // =====================================================
    // 3. UI 容器：钛金风格标题与按钮
    // =====================================================
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)winW, (float)winH });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
    ImGui::Begin("TitaniumHub", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    {
        // [标题区：TITANIUM 风格重制]
        float titleMargin = 80.0f * scale;
        ImGui::SetCursorPos({ titleMargin, titleMargin });
        ImGui::BeginGroup();
        {
            // 装饰：左侧垂直短线
            drawList->AddRectFilled({ titleMargin - 20 * scale, titleMargin + 10 * scale }, { titleMargin - 14 * scale, titleMargin + 110 * scale }, IM_COL32(0, 255, 255, 180));

            ImGui::SetWindowFontScale(4.2f * scale);
            ImGui::TextColored({ 1, 1, 1, 0.95f }, "CENTRAL");
            ImGui::SameLine();
            ImGui::TextColored({ 0, 0.9f, 1, 1 }, "DISPATCHER");

            ImGui::SetWindowFontScale(0.9f * scale);
            ImGui::TextColored({ 0.4f, 0.6f, 0.8f, 0.7f }, "QUANTUM_SIMULATION_HUB // CORE_NAVIGATION_ARRAY_v1.1.1");

            // 增加动态坐标/负载模拟
            ImGui::TextColored({ 0.3f, 0.4f, 0.5f, 0.5f }, "LATENCY: 0.002ms // LOAD: %.1f%% // GRID_REF: 0xAF44", 2.0f + sinf(t) * 0.5f);
        }
        ImGui::EndGroup();

        // =====================================================
        // 2. 核心调度网格：增加纵向结构线
        // =====================================================
        float hubW = 1400.0f * scale;
        float hubH = 620.0f * scale;
        ImVec2 tablePos = { (winW - hubW) * 0.5f, (winH - hubH) * 0.55f };
        ImGui::SetCursorPos(tablePos);

        // 绘制纵向结构梁 (在表格下方作为背景)
        for (int i = 0; i < 4; ++i) {
            float x = tablePos.x + i * (hubW / 3.0f);
            drawList->AddLine({ x, tablePos.y - 20 * scale }, { x, tablePos.y + hubH }, IM_COL32(255, 255, 255, 15), 1.0f);
        }

        if (ImGui::BeginTable("HubGrid", 3, ImGuiTableFlags_SizingFixedFit, { hubW, hubH })) {
            ImGui::TableSetupColumn("C1", ImGuiTableColumnFlags_WidthFixed, 440.0f * scale);
            ImGui::TableSetupColumn("C2", ImGuiTableColumnFlags_WidthFixed, 440.0f * scale);
            ImGui::TableSetupColumn("C3", ImGuiTableColumnFlags_WidthFixed, 440.0f * scale);
            ImGui::TableNextRow();

            // --- 核心按钮 Lambda 优化 ---
            static int btnCount = 0; btnCount = 0; // 用于生成自动 ID
            auto HubButton = [&](const char* label, const char* desc, AppScreen target, ImU32 accentCol) {
                btnCount++;
                ImGui::PushID(label);
                ImVec2 size = { 410.0f * scale, 135.0f * scale };
                ImVec2 p = ImGui::GetCursorScreenPos();
                bool hovered = ImGui::IsMouseHoveringRect(p, { p.x + size.x, p.y + size.y });
                bool active = (state.currentScreen == target);

                if (ImGui::InvisibleButton("##btn", size)) {
                    state.currentScreen = target;
                    isIntroMode = (target != AppScreen::HamStation && target != AppScreen::LifeGame);
                }

                // 1. 按钮背景渲染 (增加扫描线纹理)
                ImU32 bgCol = active ? IM_COL32(0, 180, 255, 45) : (hovered ? IM_COL32(40, 55, 70, 120) : IM_COL32(15, 20, 25, 180));
                drawList->AddRectFilled(p, { p.x + size.x, p.y + size.y }, bgCol, 2.0f * scale);

                // 绘制内部微弱扫描线
                for (float ly = 4; ly < size.y; ly += 6) {
                    drawList->AddLine({ p.x + 2, p.y + ly }, { p.x + size.x - 2, p.y + ly }, IM_COL32(255, 255, 255, 5));
                }

                // 2. 左侧状态指示条
                float barAlpha = active ? (0.7f + 0.3f * sinf(t * 5)) : (hovered ? 0.6f : 0.1f);
                drawList->AddRectFilled({ p.x, p.y }, { p.x + 4 * scale, p.y + size.y },
                    IM_COL32(0, 255, 255, (int)(barAlpha * 255)));

                // 3. 边框与边角
                drawList->AddRect(p, { p.x + size.x, p.y + size.y }, hovered ? accentCol : IM_COL32(255, 255, 255, 25), 2.0f * scale);

                if (hovered) {
                    // 动态收缩的边角
                    float offset = 2.0f * sinf(t * 8.0f);
                    float lens = 25.0f * scale;
                    ImVec2 pOff = { p.x + offset, p.y + offset };
                    ImVec2 sOff = { p.x + size.x - offset, p.y + size.y - offset };
                    drawList->AddLine(pOff, { pOff.x + lens, pOff.y }, accentCol, 2.0f);
                    drawList->AddLine(pOff, { pOff.x, pOff.y + lens }, accentCol, 2.0f);
                    drawList->AddLine(sOff, { sOff.x - lens, sOff.y }, accentCol, 2.0f);
                    drawList->AddLine(sOff, { sOff.x, sOff.y - lens }, accentCol, 2.0f);
                }

                // 4. 文字排版优化
                // [编号显示]
                char idBuf[8]; sprintf(idBuf, "%02d", btnCount);
                drawList->AddText(nullptr, 14.0f * scale, { p.x + size.x - 35 * scale, p.y + 10 * scale }, IM_COL32(255, 255, 255, 60), idBuf);

                ImGui::SetCursorScreenPos({ p.x + 25 * scale, p.y + 30 * scale });
                ImGui::SetWindowFontScale(1.6f * scale);
                ImGui::TextColored(active ? ImVec4(0, 1, 1, 1) : (hovered ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.85f)), label);

                ImGui::SetCursorScreenPos({ p.x + 25 * scale, p.y + 78 * scale });
                ImGui::SetWindowFontScale(0.85f * scale);
                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + size.x - 40 * scale);
                ImGui::TextColored({ 0.5f, 0.6f, 0.7f, 0.8f }, "%s", desc);
                ImGui::PopTextWrapPos();
                ImGui::SetWindowFontScale(1.0f);

                ImGui::Dummy({ 0, 20 * scale });
                ImGui::PopID();
                };

            // --- 按钮列分配 (Logic Grouping) ---
            ImGui::TableSetColumnIndex(0);
            ImGui::TextColored({ 1.0f, 0.3f, 0.3f, 1.0f }, ">> INFRASTRUCTURE.LOG");
            ImGui::Separator(); ImGui::Spacing();
            HubButton("AUTH_LOGO", "Initialize secure kernel handshake and identity verification nodes.", AppScreen::Intro1, IM_COL32(0, 255, 255, 180));
            HubButton("INTERFACE", "Neural-link topology and system-wide input mapping configuration.", AppScreen::Intro2, IM_COL32(0, 255, 255, 180));
            HubButton("CORE_DIAG", "Real-time GPGPU throughput analysis and hardware health telemetry.", AppScreen::CudaDiagno, IM_COL32(0, 255, 255, 180));

            ImGui::TableSetColumnIndex(1);
            ImGui::TextColored({ 0, 0.8f, 1, 0.8f }, ">> ANALYTICS.UNIT");
            ImGui::Separator(); ImGui::Spacing();
            HubButton("BIOME_HOST_CPU", "Heuristic cellular evolution via sequential x64 logic processing.", AppScreen::LifeGame, IM_COL32(0, 255, 255, 220));
            HubButton("BIOME_ACCEL_GPU", "Massively parallel biosphere synthesis utilizing CUDA tensor cores.", AppScreen::LifeGame2, IM_COL32(0, 255, 255, 220));
            
            ImGui::TableSetColumnIndex(2);
            ImGui::TextColored({ 0, 1, 0.6f, 1 }, ">> COMPUTE.ENGINES");
            ImGui::Separator(); ImGui::Spacing();
            HubButton("RF_STATION", "Multi-band RF signal interception and automated spectrum analysis.", AppScreen::HamStation, IM_COL32(0, 255, 255, 220));

            ImGui::EndTable();
        }

        // --- 底部装饰栏 ---
        float footY = winH - 60.0f * scale;
        drawList->AddLine({ 40 * scale, footY }, { winW - 40 * scale, footY }, IM_COL32(255, 255, 255, 20));

        char timeBuf[32];
        time_t now = time(nullptr);
        tm t_struct; localtime_s(&t_struct, &now);
        strftime(timeBuf, sizeof(timeBuf), "SYSTEM_UTC: %H:%M:%S", &t_struct);
        drawList->AddText(nullptr, 20.0f * scale, { 60 * scale, footY + 15 * scale }, IM_COL32(255, 255, 255, 100), timeBuf);

        // --- 底部右侧：开发者与协议信息 (换行排列) ---
        float infoX = winW - 250 * scale; // 起始 X 坐标
        float firstLineY = footY + 8 * scale;  // 第一行偏移
        float secondLineY = footY + 28 * scale; // 第二行偏移（增加约 20 像素间距）

        // 第一行：开发者
        drawList->AddText(nullptr, 20.0f * scale, { infoX, firstLineY }, IM_COL32(0, 255, 255, 180), "AUTHOR_REF : XIAO YU");

        // 第二行：协议（颜色稍微调暗，字体略微缩小，增加精密感）
        drawList->AddText(nullptr, 16.0f * scale, { infoX, secondLineY }, IM_COL32(0, 255, 255, 100), "LIC_TYPE   : MIT LICENSE");
}
    ImGui::End();
    ImGui::PopStyleVar();
}
// 界面1，初始版本界面
void RenderIntroScreen1(SimState& state, int winW, int winH, bool& isIntroMode, AsyncLoader& loader) {
    // 1. 计算缩放因子 (以 1920x1080 为基准)
    float scale = (float)winW / 1920.0f;
    // 强制限制缩放范围，防止极端分辨率下过大或过小
    scale = fmax(0.5f, fmin(scale, 2.0f));

    // 背景色
    ImVec4 bgColor = ImVec4(0.12f, 0.13f, 0.18f, 0.2f);
 
    // 2. 全屏窗口设置
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)winW, (float)winH));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);

    if (ImGui::Begin("IntroFullscreen", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNav)) {

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // === 基础布局变量 ===
        float commonMargin = 80.0f * scale;
        float headerY = commonMargin;
        float blockY = commonMargin + 280.0f * scale;

        // =====================================================
        // 3. 渲染：对称钛金背景
        // =====================================================
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 screenCenter(winW * 0.5f, winH * 0.52f);
        ImVec2 mPosRel = ImVec2(io.MousePos.x - screenCenter.x, io.MousePos.y - screenCenter.y);
        //ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        float subStep = 25.0f * scale, mainStep = 125.0f * scale;
        auto DrawSymmetricGrid = [&](float step, ImU32 col, bool drawDots) {
            for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
                drawList->AddLine(ImVec2(screenCenter.x + x, 0), ImVec2(screenCenter.x + x, (float)winH), col, 1.0f);
                if (x > 0) drawList->AddLine(ImVec2(screenCenter.x - x, 0), ImVec2(screenCenter.x - x, (float)winH), col, 1.0f);
            }
            for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
                drawList->AddLine(ImVec2(0, screenCenter.y + y), ImVec2((float)winW, screenCenter.y + y), col, 1.0f);
                if (y > 0) drawList->AddLine(ImVec2(0, screenCenter.y - y), ImVec2((float)winW, screenCenter.y - y), col, 1.0f);
            }
            if (drawDots) {
                for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
                    for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
                        drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                        if (x > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                        if (y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                        if (x > 0 && y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    }
                }
            }
            };
        DrawSymmetricGrid(subStep, IM_COL32(255, 255, 255, 5), false);
        DrawSymmetricGrid(mainStep, IM_COL32(255, 255, 255, 15), true);


        // [区域 1] 左上角：主标题
        ImGui::SetCursorPos(ImVec2(commonMargin, headerY));
        ImGui::BeginGroup();
        {
            ImGui::SetWindowFontScale(4.2f * scale);
            ImGui::TextColored(ImVec4(0.0f, 0.85f, 1.0f, 1.0f), "ELASTIC WAVE");
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SIMULATION GPU");

            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 0.85f, 1.0f, 0.5f));
            ImGui::Separator();
            ImGui::PopStyleColor();

            ImGui::Spacing();
            ImGui::SetWindowFontScale(1.5f * scale);
            ImGui::TextColored(ImVec4(0.6f, 0.7f, 0.8f, 1.0f), "High-Performance Finite-Difference Solver v3.0");
        }
        ImGui::EndGroup();

        // [区域 2] 右上角：技术参数
        float rightBlockWidth = 600.0f * scale;
        ImGui::SetCursorPos(ImVec2(winW - rightBlockWidth - commonMargin, blockY));
        ImGui::BeginGroup();
        {
            ImGui::SetWindowFontScale(1.8f * scale);
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.6f, 1.0f), "// SYSTEM SPECS");

            ImGui::SetWindowFontScale(1.0f * scale);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.85f, 0.9f, 1.0f));
            ImGui::Columns(2, "SpecsCols", false);
            ImGui::SetColumnWidth(0, 200.0f * scale);

            float lineSpacing = 8.0f * scale;
            auto DrawSpec = [&](const char* label, const char* value, bool isColored = false, ImVec4 color = ImVec4(1, 1, 1, 1)) {
                ImGui::Text("%s", label); ImGui::NextColumn();
                if (isColored) ImGui::TextColored(color, "%s", value);
                else ImGui::Text("%s", value);
                ImGui::NextColumn();
                ImGui::Dummy(ImVec2(0, lineSpacing)); ImGui::NextColumn(); ImGui::Dummy(ImVec2(0, lineSpacing)); ImGui::NextColumn();
                };

            DrawSpec("Architecture:", "OpenGL 4.3 Compute", true, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));
            DrawSpec("Time Scheme:", "Leapfrog (2nd Order)");
            DrawSpec("Space Scheme:", "Staggered Grid (L8)");
            DrawSpec("Boundary:", "CPML / Rigid / Free");
            DrawSpec("Resolution:", "Dynamic Grid");
            DrawSpec("Developer:", "XIAO YU", true, ImVec4(1.0f, 0.85f, 0.4f, 1.0f));
            DrawSpec("License:", "MIT License", true, ImVec4(0.6f, 1.0f, 0.6f, 1.0f));
            DrawSpec("Copyright:", "(c) 2025 All Rights Reserved");

            ImGui::Columns(1);
            ImGui::PopStyleColor();
        }
        ImGui::EndGroup();

        // [区域 3] 中间左侧：功能列表
        ImGui::SetCursorPos(ImVec2(commonMargin, blockY));
        ImGui::BeginGroup();
        {
            ImGui::SetWindowFontScale(1.8f * scale);
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "// CORE FEATURES");
            ImGui::SetWindowFontScale(1.0f * scale);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.95f, 1.0f, 1.0f));

            float itemH = 15.0f * scale;
            const char* features[] = { "GPU-Accelerated Compute Shader Kernel", "High-Order Staggered Grid FDM (L8)", "CPML Absorbing Boundary Conditions", "Heterogeneous Media (Solid / Fluid / Vacuum)", "Real-time Multi-Component Wavefield Rendering", "Dynamic Source Injection & Seismic Recording" };

            for (const char* f : features) {
                ImGui::Bullet(); ImGui::Text("%s", f);
                ImGui::Dummy(ImVec2(0, itemH));
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndGroup();
        // [区域 5] 高级多节点波动曲线
        {
            double t = ImGui::GetTime();
            float startX = commonMargin- commonMargin;
            float endX = winW;
            float waveAreaWidth = endX - startX;

            float baseCenterY = winH - 250.0f * scale;
            int numLayers = 16;
            int segments = 50; // 每一条曲线包含 20 个段，增加曲线的精细度

            for (int i = 0; i < numLayers; ++i) {
                float layerOffset = ((float)i / (float)numLayers - 0.5f) * (150.0f * scale);
                float currentY = baseCenterY + layerOffset + sin(t * 0.3f) * 20.0f * scale;

                // 颜色定义
                float hue = fmod(t * 0.08f + (float)i / (float)numLayers, 1.0f);
                ImU32 col = GetColorFromHSV(hue, 0.6f, 0.9f, 0.4f - (i * 0.02f));

                // 开始构建路径
                drawList->PathClear();
                for (int j = 0; j <= segments; ++j) {
                    float ratio = (float)j / (float)segments;
                    float x = startX + waveAreaWidth * ratio;

                    // 多个波形叠加：sin(频率 * x + 时间)
                    // 频率随 j 增加而变化，产生多变起伏
                    float wave = sin(ratio * 12.0f + t + (float)i * 0.5f) * (30.0f * scale) +
                        cos(ratio * 12.0f + t * 0.5f) * (10.0f * scale);

                    drawList->PathLineTo(ImVec2(x, currentY + wave));
                }

                // 绘制路径 (细线条)
                float thickness = (0.6f + (float)i * 0.05f) * scale;
                drawList->PathStroke(col, false, thickness);
            }
        }

        // =================================================
    // [区域 4] 左下角：时间与状态提示
    // =================================================
        // 定义一个统一的底部对齐参考高度 (距离底部的固定距离)
        float bottomPadding = commonMargin * 0.5f;
        float bottomLineY = winH - bottomPadding;
        {
            std::time_t now = std::time(nullptr);
            struct tm now_tm;
            localtime_s(&now_tm, &now);
            char timeBuf[64];
            std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &now_tm);

            // 测量组的高度以便向上偏移，使组的底部对齐到底线
            // 简单的做法是直接设定高度
            ImGui::SetCursorPos(ImVec2(commonMargin, bottomLineY - 100.0f * scale));

            ImGui::BeginGroup();
            {
                ImGui::SetWindowFontScale(3.8f * scale);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", timeBuf);

                ImGui::SetWindowFontScale(1.2f * scale);
                float blinkAlpha = 0.3f + 0.7f * (0.5f * (1.0f + sin(ImGui::GetTime() * 4.0f)));
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, blinkAlpha), "SYSTEM ONLINE");
            }
            ImGui::EndGroup();
        }

        // =================================================
        // [区域 5] 右下角：启动按钮
        // =================================================
        {
            float btnWidth = 260.0f * scale;
            float btnHeight = 65.0f * scale;

            // 让按钮组底部对齐到 bottomLineY
            // posY = 目标底部线 - 按钮组总高度
            float groupHeight = btnHeight + 30.0f * scale; // 按钮高度 + 提示文字高度 + 间距
            float posY = bottomLineY - groupHeight;
            float posX = winW - btnWidth - commonMargin;

            ImGui::SetCursorPos(ImVec2(posX, posY));
            ImGui::BeginGroup();
            {
                // ... (按钮代码保持不变) ...
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 32.0f * scale);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.15f, 0.15f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.05f, 0.05f, 1.0f));

                if (loader.isLoading || loader.isDataReady) {
                    ImGui::BeginDisabled();
                    ImGui::Button("LOADING...", ImVec2(btnWidth, btnHeight));
                    ImGui::EndDisabled();
                }
                else {
                    if (ImGui::Button("LAUNCH SIMULATION", ImVec2(btnWidth, btnHeight))) 
                    { state.currentScreen = AppScreen::Intro0; isIntroMode = false; }
                }
                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar();

                // 提示文字
                ImGui::Dummy(ImVec2(0, 8 * scale));
                std::string hint = "PRESS [SPACE] TO INITIALIZE";
                float textWidth = ImGui::CalcTextSize(hint.c_str()).x;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (btnWidth - textWidth * 0.8f) * 0.5f);
                ImGui::SetWindowFontScale(0.9f * scale);
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%s", hint.c_str());
            }
            ImGui::EndGroup();
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(3);
}

struct SciParticle {
    ImVec2 pos;
    ImVec2 vel;
    ImU32  color;
    float  phase;
    float  orbitSize;

    static const int ABS_MAX_TRAIL = 2400;
    ImVec2 trail[ABS_MAX_TRAIL];
    int trail_ptr = 0;
    int trail_count = 0;

    void AddTrail(ImVec2 p) {
        trail[trail_ptr] = p;
        trail_ptr = (trail_ptr + 1) % ABS_MAX_TRAIL;
        if (trail_count < ABS_MAX_TRAIL) trail_count++;
    }
};
// 界面2,粒子路径变换
void RenderIntroScreen2(SimState& state, int winW, int winH, bool& isIntroMode, AsyncLoader& loader) {
    float scale = (float)winW / 1920.0f;
    double t = ImGui::GetTime();
    ImGuiIO& io = ImGui::GetIO();
    int frameCount = ImGui::GetFrameCount();

    // =====================================================
    // 1. 状态管理
    // =====================================================
    static std::vector<SciParticle> particles;
    static std::vector<ImVec2> static_noise;
    static bool initialized = false;
    static float lastTime = 0;
    static int pathMode = 0;
    static int colorTheme = 3;
    static int targetParticleCount = 80;
    static int currentTrailLength = 60;
    static float simSpeed = 1.0f; // 默认速度倍率

    static float baseOrbitSize = 350.0f; // 基础轨道大小
    static float orbitWidth = 200.0f;    // 轨道宽度（离散度）

    // --- [新增] 利萨如图形专用参数 ---
    static float lis_a = 3.0f;     // 水平频率
    static float lis_b = 2.0f;     // 垂直频率
    static float lis_A = 0.8f;     // 水平振幅倍率
    static float lis_B = 0.8f;     // 垂直振幅倍率
    static float lis_delta = 0.5f; // 相位差

    const ImU32 palette[] = {
        IM_COL32(0, 255, 240, 255), IM_COL32(170, 100, 255, 255), IM_COL32(255, 185, 0, 255),
        IM_COL32(50, 255, 120, 255), IM_COL32(255, 60, 100, 255), IM_COL32(0, 130, 255, 255)
    };

    float dt = (float)t - lastTime;
    if (dt > 0.05f) dt = 0.016f;
    lastTime = (float)t;

    // 动态初始化/数量调整
    if (!initialized || (int)particles.size() != targetParticleCount) {
        size_t oldSize = particles.size();
        particles.resize(targetParticleCount);
        for (size_t i = oldSize; i < (size_t)targetParticleCount; i++) {
            particles[i].pos = ImVec2(((float)(rand() % 1000) - 500) * scale, ((float)(rand() % 800) - 400) * scale);
            particles[i].vel = ImVec2(0, 0);
            particles[i].phase = (float)(i * 0.17f);
            // 初始化时使用 rand() 产生初始随机分布
            particles[i].orbitSize = (baseOrbitSize + (float)(rand() % (int)(orbitWidth + 1))) * scale;
            particles[i].trail_count = 0;
            // 初始分配颜色
            if (colorTheme == 0) particles[i].color = palette[i % 3];
            else if (colorTheme == 1) particles[i].color = (i % 2 == 0) ? palette[0] : palette[5];
            else if (colorTheme == 2) particles[i].color = (i % 2 == 0) ? palette[2] : palette[4];
            else particles[i].color = palette[i % 6];
        }
        if (static_noise.empty()) {
            for (int i = 0; i < 2000; i++) static_noise.push_back(ImVec2((float)(rand() % winW), (float)(rand() % winH)));
        }
        initialized = true;
    }

    ImVec2 screenCenter(winW * 0.5f, winH * 0.52f);
    ImVec2 mPosRel = ImVec2(io.MousePos.x - screenCenter.x, io.MousePos.y - screenCenter.y);
    bool mouseActive = (io.MousePos.x > 0 && io.MousePos.y > 0);
    const float influenceRadius = 100.0f * scale;

    // =====================================================
    // 2. 物理模拟 (7种数学路径)
    // =====================================================
    for (int i = 0; i < (int)particles.size(); i++) {
        ImVec2 acc = { 0, 0 };
        //float p_t = t * 0.7f + particles[i].phase;
        float p_t = (float)t * 0.7f * simSpeed + particles[i].phase;
        float a = particles[i].orbitSize;
        ImVec2 targetPos;

        switch (pathMode) {
        case 1: targetPos = { a * 0.7f * cosf(p_t), a * 0.7f * sinf(p_t) }; break; // Circle
        case 2: targetPos = { a * 0.8f * powf(cosf(p_t), 3.0f), a * 0.8f * powf(sinf(p_t), 3.0f) }; break; // Astroid
        case 3: targetPos = { a * 0.05f * (16 * powf(sinf(p_t), 3.0f)), -a * 0.05f * (13 * cosf(p_t) - 5 * cosf(2 * p_t) - 2 * cosf(3 * p_t) - cosf(4 * p_t)) }; break; // Heart
        case 4: // LISSAJOUS
            targetPos = {
                particles[i].orbitSize * lis_A * sinf(lis_a * p_t + lis_delta),
                particles[i].orbitSize * lis_B * sinf(lis_b * p_t)
            };
            break; // Lissajous
        case 5: { float r = a * 0.8f * cosf(4.0f * p_t); targetPos = { r * cosf(p_t), r * sinf(p_t) }; } break; // Rose
        case 6: {
            float angleOffset = (float)(i % 3) * 2.094f;
            float xl = a * 0.9f * cosf(p_t), yl = a * 0.3f * sinf(p_t);
            targetPos = { xl * cosf(angleOffset) - yl * sinf(angleOffset), xl * sinf(angleOffset) + yl * cosf(angleOffset) };
        } break; // Atomic
        case 7: {
            // 蝴蝶曲线公式
            float exp_cos = expf(cosf(p_t));
            float r = exp_cos - 2.0f * cosf(4.0f * p_t) - powf(sinf(p_t / 12.0f), 5.0f);
            targetPos = { a * 0.4f * r * sinf(p_t), -a * 0.4f * r * cosf(p_t) };
        } break;
        case 8: {
            float R = 5.0f, r = 3.0f, d = 5.0f; // 调整这些常数可以改变形状
            float x = (R - r) * cosf(p_t) + d * cosf((R - r) / r * p_t);
            float y = (R - r) * sinf(p_t) - d * sinf((R - r) / r * p_t);
            targetPos = { x * a * 0.1f, y * a * 0.1f };
        } break;
        case 9: {
            targetPos = {
                a * 0.5f * (2.0f * cosf(p_t) + cosf(2.0f * p_t)),
                a * 0.5f * (2.0f * sinf(p_t) - sinf(2.0f * p_t))
            };
        } break;
        case 10: {
            float growth = fmodf(p_t * 0.5f, 1.0f); // 让螺线有循环生长的效果
            float r = a * growth;
            float theta = p_t * 5.0f; // 增加旋转圈数
            targetPos = { r * cosf(theta), r * sinf(theta) };
        } break;
        case 11: {
            // 使用粒子索引来固定位置，或者用 p_t 模拟流动
            float r = a * 0.5f * sqrtf(p_t);
            targetPos = { r * cosf(p_t * 2.399f), r * sinf(p_t * 2.399f) }; // 2.399弧度是黄金角
        } break;
        default: { float d = 1.0f + powf(sinf(p_t), 2.0f); targetPos = { (a * cosf(p_t)) / d, (a * sinf(p_t) * cosf(p_t)) / d }; } break; // infinity
        }

        float dPM = sqrtf(mPosRel.x * mPosRel.x + mPosRel.y * mPosRel.y); // Mouse dist
        float dParticleToMouse = sqrtf(powf(mPosRel.x - particles[i].pos.x, 2.0f) + powf(mPosRel.y - particles[i].pos.y, 2.0f));
        float mouseWeight = (mouseActive && dParticleToMouse < influenceRadius) ? powf(1.0f - (dParticleToMouse / influenceRadius), 2.0f) : 0.0f;

        if (mouseWeight > 0.01f) {
            ImVec2 diffM = { mPosRel.x - particles[i].pos.x, mPosRel.y - particles[i].pos.y };
            acc.x += (diffM.x / (dParticleToMouse + 1.0f)) * 1800.0f * scale * mouseWeight;
            acc.y += (diffM.y / (dParticleToMouse + 1.0f)) * 1800.0f * scale * mouseWeight;
            particles[i].vel.x *= (1.0f - 0.15f * mouseWeight); particles[i].vel.y *= (1.0f - 0.15f * mouseWeight);
        }

        ImVec2 diffP = { targetPos.x - particles[i].pos.x, targetPos.y - particles[i].pos.y };
        acc.x += diffP.x * 25.0f * scale * (1.0f - mouseWeight);
        acc.y += diffP.y * 25.0f * scale * (1.0f - mouseWeight);

        // 修改后：引入 simSpeed
        float adaptiveDt = dt * simSpeed;

        // 使用之前讨论的帧率无关阻尼逻辑 (0.028 是 1.0 - 0.972 的补数)
        float dampingFactor = powf(0.972f, adaptiveDt * 240.0f);

        particles[i].vel.x = (particles[i].vel.x + acc.x * adaptiveDt) * dampingFactor;
        particles[i].vel.y = (particles[i].vel.y + acc.y * adaptiveDt) * dampingFactor;

        particles[i].pos.x += particles[i].vel.x * adaptiveDt;
        particles[i].pos.y += particles[i].vel.y * adaptiveDt;

        if (frameCount % 2 == 0) particles[i].AddTrail(particles[i].pos);
    }

    // =====================================================
    // 3. 渲染：对称钛金背景
    // =====================================================
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilledMultiColor(ImVec2(0, 0), ImVec2(winW, winH),
        IM_COL32(35, 42, 52, 255), IM_COL32(35, 42, 52, 255), IM_COL32(15, 17, 20, 255), IM_COL32(15, 17, 20, 255));
    for (auto& np : static_noise) drawList->AddRectFilled(np, ImVec2(np.x + 1, np.y + 1), IM_COL32(255, 255, 255, 6));

    float subStep = 25.0f * scale, mainStep = 125.0f * scale;
    auto DrawSymmetricGrid = [&](float step, ImU32 col, bool drawDots) {
        for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
            drawList->AddLine(ImVec2(screenCenter.x + x, 0), ImVec2(screenCenter.x + x, (float)winH), col, 1.0f);
            if (x > 0) drawList->AddLine(ImVec2(screenCenter.x - x, 0), ImVec2(screenCenter.x - x, (float)winH), col, 1.0f);
        }
        for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
            drawList->AddLine(ImVec2(0, screenCenter.y + y), ImVec2((float)winW, screenCenter.y + y), col, 1.0f);
            if (y > 0) drawList->AddLine(ImVec2(0, screenCenter.y - y), ImVec2((float)winW, screenCenter.y - y), col, 1.0f);
        }
        if (drawDots) {
            for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
                for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
                    drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (x > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (x > 0 && y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                }
            }
        }
        };
    DrawSymmetricGrid(subStep, IM_COL32(255, 255, 255, 5), false);
    DrawSymmetricGrid(mainStep, IM_COL32(255, 255, 255, 15), true);

    if (mouseActive) {
        float pulse = 0.5f + 0.5f * sin((float)t * 4.0f);
        drawList->AddCircle(io.MousePos, influenceRadius, IM_COL32(0, 255, 255, (int)(25 + 15 * pulse)), 64, 1.0f);
    }

    // 绘制粒子
    int renderTrailLimit = std::min(currentTrailLength, SciParticle::ABS_MAX_TRAIL);
    for (auto& p : particles) {
        int actualPoints = std::min(p.trail_count, renderTrailLimit);
        if (actualPoints > 2) {
            for (int k = 1; k < actualPoints; k++) {
                int idx1 = (p.trail_ptr - actualPoints + k - 1 + SciParticle::ABS_MAX_TRAIL) % SciParticle::ABS_MAX_TRAIL;
                int idx2 = (p.trail_ptr - actualPoints + k + SciParticle::ABS_MAX_TRAIL) % SciParticle::ABS_MAX_TRAIL;
                float prog = (float)k / actualPoints;
                drawList->AddLine(ImVec2(screenCenter.x + p.trail[idx1].x, screenCenter.y + p.trail[idx1].y),
                    ImVec2(screenCenter.x + p.trail[idx2].x, screenCenter.y + p.trail[idx2].y),
                    (p.color & 0x00FFFFFF) | ((ImU32)(prog * 120) << 24), 2.2f * scale * prog);
                drawList->AddLine(ImVec2(screenCenter.x + p.trail[idx1].x, screenCenter.y + p.trail[idx1].y),
                    ImVec2(screenCenter.x + p.trail[idx2].x, screenCenter.y + p.trail[idx2].y),
                    IM_COL32(255, 255, 255, (ImU32)(prog * 140)), 0.6f * scale * prog);
            }
        }
        ImVec2 cp = { screenCenter.x + p.pos.x, screenCenter.y + p.pos.y };
        drawList->AddCircleFilled(cp, 3.2f * scale, IM_COL32_WHITE);
        drawList->AddCircleFilled(cp, 7.5f * scale, (p.color & 0x00FFFFFF) | 0x66000000);
    }

    // =====================================================
    // 4. UI 交互层 (包含颜色切换与监控)
    // =====================================================
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)winW, (float)winH));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    if (ImGui::Begin("TitaniumOverlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
        float margin = 80.0f * scale;

        // --- 左上角: 标题 + 路径选择 + 颜色选择 ---
        ImGui::SetCursorPos(ImVec2(margin, margin));
        ImGui::BeginGroup();
        {
            ImGui::SetWindowFontScale(4.2f * scale);
            ImGui::TextColored(ImVec4(1, 1, 1, 0.95f), "TITANIUM"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0, 0.9f, 1, 1), "SOLVER");

            // 路径按钮组
            ImGui::SetWindowFontScale(0.8f * scale); ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
            const char* p_labels[] = { "INF", "CIR", "STR", "HRT", "LIS", "ROS", "ATM", "BTF", "HYP", "DEL", "SPI", "FER" };
            for (int i = 0; i < 12; i++) {
                if (pathMode == i) ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 1, 1, 1));
                else ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.2f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                if (ImGui::Button(p_labels[i], ImVec2(45 * scale, 25 * scale))) pathMode = i;
                ImGui::PopStyleVar(); ImGui::PopStyleColor(); ImGui::SameLine();
            }

            ImGui::Dummy(ImVec2(20 * scale, 0)); 
            //ImGui::SameLine();

            // 颜色主题按钮组 (回归)
            const char* c_labels[] = { "CLASSIC", "COLD", "WARM", "RAINBOW" };
            for (int i = 0; i < 4; i++) {
                if (colorTheme == i) ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 1, 1, 1));
                else ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.2f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                if (ImGui::Button(c_labels[i], ImVec2(75 * scale, 25 * scale))) {
                    colorTheme = i;
                    for (size_t j = 0; j < particles.size(); j++) {
                        if (colorTheme == 0) particles[j].color = palette[j % 3];
                        else if (colorTheme == 1) particles[j].color = (j % 2 == 0) ? palette[0] : palette[5];
                        else if (colorTheme == 2) particles[j].color = (j % 2 == 0) ? palette[2] : palette[4];
                        else particles[j].color = palette[j % 6];
                    }
                }
                ImGui::PopStyleVar(); ImGui::PopStyleColor(); ImGui::SameLine();
            }
            ImGui::PopStyleVar();
        }
        ImGui::EndGroup();

        // --- 左下角: 监控面板 (参数与监控回归) ---
        ImGui::SetCursorPos(ImVec2(margin, (float)winH - margin - 230.0f * scale));
        ImGui::BeginGroup();
        {
            ImGui::SetWindowFontScale(0.8f * scale);
            ImGui::TextColored(ImVec4(0, 1, 0.8f, 0.8f), ">> NODE_ANALYSIS: %d UNITS", (int)particles.size());
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 1, 1, 0.05f));
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0, 1, 1, 0.6f));

            ImGui::SetNextItemWidth(250.0f * scale); ImGui::SliderInt("##count", &targetParticleCount, 10, 240, "DENSITY: %d");
            ImGui::SetNextItemWidth(250.0f * scale); ImGui::SliderInt("##trails", &currentTrailLength, 10, 240, "SPECTRUM: %d");
            ImGui::SetNextItemWidth(250.0f * scale);ImGui::SliderFloat("##speed", &simSpeed, 0.1f, 5.0f, "SIM_SPEED: %.1fx");
            ImGui::SetNextItemWidth(250.0f * scale);
            bool orbitChanged = false;
            // 轨道半径调节
            if (ImGui::SliderFloat("##orbit", &baseOrbitSize, 50.0f, 1000.0f, "ORBIT_RADIUS: %.0f")) orbitChanged = true;

            ImGui::SetNextItemWidth(250.0f * scale);
            // 轨道宽度（离散度）调节
            if (ImGui::SliderFloat("##width", &orbitWidth, 0.0f, 500.0f, "ORBIT_WIDTH: %.0f")) orbitChanged = true;

            // 如果任何一个参数改变，立即同步到所有粒子
            if (orbitChanged) {
                for (size_t i = 0; i < (int)particles.size(); i++) {
                    particles[i].orbitSize = (baseOrbitSize + (float)(rand() % (int)(orbitWidth + 1))) * scale;
                }
            }
            ImGui::PopStyleColor(2);
            // --- [插入位置] 在 ImGui::SliderFloat("##speed", &simSpeed...) 之后 ---

            if (pathMode == 4) {
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0, 1, 1, 0.6f), "LISSAJOUS_SIGNAL_TUNING:");

                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 1, 1, 0.05f));
                ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0, 1, 1, 0.6f));

                // 频率调节
                ImGui::SetNextItemWidth(250.0f * scale);
                ImGui::SliderFloat("##lisa", &lis_a, 1.0f, 12.0f, "FREQ_X (a): %.1f");
                ImGui::SetNextItemWidth(250.0f * scale);
                ImGui::SliderFloat("##lisb", &lis_b, 1.0f, 12.0f, "FREQ_Y (b): %.1f");

                // 振幅调节
                ImGui::SetNextItemWidth(250.0f * scale);
                ImGui::SliderFloat("##lisA", &lis_A, 0.1f, 2.0f, "AMP_X (A): %.2f");
                ImGui::SetNextItemWidth(250.0f * scale);
                ImGui::SliderFloat("##lisB", &lis_B, 0.1f, 2.0f, "AMP_Y (B): %.2f");

                // 相位调节
                ImGui::SetNextItemWidth(250.0f * scale);
                ImGui::SliderFloat("##lisD", &lis_delta, 0.0f, 6.28f, "PHASE (delta): %.2f");

                ImGui::PopStyleColor(2);
                ImGui::Spacing();
                // 使用细边框和深色背景，保持工业感
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.1f, 0.1f, 0.4f)); // 淡淡的暗红色提示这是重置
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.2f, 0.2f, 0.4f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.6f, 0.8f));

                if (ImGui::Button("RESET_LISSAJOUS_CORE", ImVec2(250.0f * scale, 30.0f * scale))) {
                    // 恢复初始默认值
                    lis_a = 3.0f;
                    lis_b = 2.0f;
                    lis_A = 0.8f;
                    lis_B = 0.8f;
                    lis_delta = 0.5f;
                }

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(1);
            }
            ImGui::SetWindowFontScale(0.85f * scale);
            const char* p_names[] = { "LEMNISCATE", "CIRCULAR", "ASTROID", "CARDIOID", "LISSAJOUS", "ROSE", "ATOMIC", "BUTTERFLY", "HYPOTROCHOID", "DELTOID", "SPIRAL", "FERMAT" };
            ImGui::TextColored(ImVec4(0.4f, 0.6f, 0.8f, 0.9f), "ACTIVE_PATH: %s", p_names[pathMode]);
            ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.6f, 0.7f), "M_FIELD_COORDINATE: X[%.1f] Y[%.1f]", mPosRel.x, mPosRel.y);
            ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.6f, 0.5f), "GRAVITY_WELL_STATUS: %s", mouseActive ? "INTERRUPTED" : "STABLE_AUTO");
        }
        ImGui::EndGroup();
        // =====================================================
// [右下角辅助控制] 渲染频率与垂直同步
// =====================================================
        {
            float btnW = 320.0f * scale;
            float margin = 80.0f * scale;
            // 定位在启动按钮上方 (启动按钮高度80 + 间距15)
            ImGui::SetCursorPos(ImVec2(winW - btnW - margin, winH - margin - 80.0f * scale - 65.0f * scale));

            ImGui::BeginGroup();
            {
                // 1. 渲染频率显示
                ImGui::SetWindowFontScale(0.9f * scale);
                float fps = ImGui::GetIO().Framerate;
                ImVec4 fpsCol = (fps < 50.0f) ? ImVec4(1, 0.4f, 0.4f, 0.8f) : ImVec4(0.4f, 1, 0.7f, 0.6f);
                ImGui::TextColored(fpsCol, "RENDER_FREQ: %.1f FPS", fps);

                // 2. 垂直同步开关 (VSYNC)
                static bool vsync = true; // 实际逻辑需绑定到你的渲染器控制，如 glfwSwapInterval

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1, 1, 1, 0.05f));
                ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0, 1, 1, 1));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.2f));

                if (ImGui::Checkbox("VSYNC_STABILIZER", &vsync)) {
                    // 这里添加实际的 VSync 切换逻辑
                    glfwSwapInterval(vsync ? 1 : 0);
                }

                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Synchronize buffer swap with monitor refresh rate");

                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar(2);
            }
            ImGui::EndGroup();
        }

        // --- 右下角: 启动按钮 ---
        float btnW = 320.0f * scale, btnH = 80.0f * scale;
        ImGui::SetCursorPos(ImVec2((float)winW - btnW - margin, (float)winH - btnH - margin));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.12f, 0.15f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.7f, 1.0f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0.8f, 1.0f, 0.4f));
        if (ImGui::Button("LAUNCH SIMULATION", ImVec2(btnW, btnH))) 
        { state.currentScreen = AppScreen::Intro0; isIntroMode = false; }

        ImVec2 bMin = ImGui::GetItemRectMin(), bMax = ImGui::GetItemRectMax();
        drawList->AddLine(bMin, ImVec2(bMin.x + 15, bMin.y), IM_COL32(0, 255, 255, 200), 2.0f);
        drawList->AddLine(bMin, ImVec2(bMin.x, bMin.y + 15), IM_COL32(0, 255, 255, 200), 2.0f);
        ImGui::PopStyleColor(3); ImGui::PopStyleVar(2);
    }
    ImGui::End();
    ImGui::PopStyleColor(1); ImGui::PopStyleVar(2);
}

//界面3,生命游戏渲染CPU
void ApplyPattern(std::vector<uint8_t>& world, int w, int h, int type, int& gen) {
    std::fill(world.begin(), world.end(), 0);
    gen = 0;
    auto place = [&](int x, int y, std::vector<std::vector<int>> p) {
        for (int i = 0; i < (int)p.size(); ++i)
            for (int j = 0; j < (int)p[i].size(); ++j)
                if (p[i][j]) world[((y + i + h) % h) * w + ((x + j + w) % w)] = 1;
        };

    if (type == 1) { // 修正后的 Gosper Glider Gun (完整 9 行结构)
        place(20, 20, {
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
            {1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
            });
    }
    else if (type == 2) { // 修正后的 Pulsar (完整 13 行对称结构)
        place(w / 2 - 6, h / 2 - 6, {
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {1,0,0,0,0,1,0,1,0,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,1,1,0,0,0,1,1,1,0,0}
            });
    }
    else if (type == 3) { // LWSS (保持不变)
        for (int k = 0; k < 4; ++k) place(20 + k * 40, 20 + k * 20, { {0,1,1,1,1},{1,0,0,0,1},{0,0,0,0,1},{1,0,0,1,0} });
    }
}

void RenderLifeGameScreen(SimState& state, int winW, int winH) {
    const float scale = (float)winW / 1920.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    // --- [1. 模拟状态与视口变量] ---
    static  int gridW = 240, gridH = 135;
    static int nextGridW = 240, nextGridH = 135;
    static std::vector<uint8_t> world(gridW * gridH, 0), nextWorld(gridW * gridH, 0);
    static std::vector<float> heatMap(gridW * gridH, 0.0f);


    // --- 新增：视口控制变量 ---
    static float zoom = 1.0f;           // 缩放倍率
    static ImVec2 viewOffset = { 0, 0 };  // 平移偏移量
    static float randomDensity = 0.28f;

    // --- [2. 缩放与平移控制逻辑] ---
    if (!io.WantCaptureMouse) {
        // A. 鼠标滚轮缩放
        if (io.MouseWheel != 0.0f) {
            float oldZoom = zoom;
            zoom += io.MouseWheel * 0.1f * zoom; // 指数级缩放感
            if (zoom < 0.5f) zoom = 0.5f;        // 最小缩放限制
            if (zoom > 50.0f) zoom = 50.0f;      // 最大缩放限制

            // 核心：以鼠标当前位置为中心进行缩放
            ImVec2 mousePos = io.MousePos;
            viewOffset.x -= (mousePos.x - viewOffset.x) * (zoom / oldZoom - 1.0f);
            viewOffset.y -= (mousePos.y - viewOffset.y) * (zoom / oldZoom - 1.0f);
        }

        // B. 鼠标中键（或右键）按下平移
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
            viewOffset.x += io.MouseDelta.x;
            viewOffset.y += io.MouseDelta.y;
        }
    }
    // --- [3. 核心计算与模拟逻辑] ---
    static bool firstRun = true;
    if (firstRun) {
        for (int i = 0; i < gridW * gridH; ++i) {
            // 使用 randomDensity 变量
            world[i] = ((float)rand() / RAND_MAX < randomDensity) ? 1 : 0;
            if (world[i]) heatMap[i] = 1.0f;
        }
        firstRun = false;
    }

    static float tickRate = 0.1f, timer = 0.0f, totalSimTime = 0.0f;
    static bool paused = state.running, showHUD = true;
    static int generation = 0; // 定义代数
    static ImVec4 cellColor = ImVec4(0.54f, 0.95f, 0.3f, 1.0f);

    // 监视器采样
    static std::vector<float> history(1800, 0.0f);
    static int hIdx = 0;
    // 在 static 变量定义下方，增加一个 Lambda 函数方便调用
    auto ResizeWorld = [&](int newW, int newH) {
        gridW = newW;
        gridH = newH;
        world.assign(gridW * gridH, 0);
        nextWorld.assign(gridW * gridH, 0);
        heatMap.assign(gridW * gridH, 0.0f);
        generation = 0;
        // 同时也重置历史记录
        std::fill(history.begin(), history.end(), 0.0f);
        };

    // --- [2. 磨砂玻璃背景渲染] ---
    draw->AddRectFilledMultiColor({ 0,0 }, { (float)winW, (float)winH },
        IM_COL32(10, 35, 30, 255), IM_COL32(20, 55, 45, 255),
        IM_COL32(5, 25, 20, 255), IM_COL32(10, 30, 25, 255));
    // 磨砂白光层
    draw->AddRectFilled({ 0,0 }, { (float)winW, (float)winH }, IM_COL32(200, 255, 230, 15));

    // 1. 提前计算当前分辨率下的细胞像素宽高
    // --- [4. 渲染映射逻辑] ---
    // 基础细胞大小
    float baseCw = (float)winW / gridW;
    float baseCh = (float)winH / gridH;

    // 实际渲染时的细胞大小（受 zoom 影响）
    float cw = baseCw * zoom;
    float ch = baseCh * zoom;

    // 绘制背景与网格 (仅在网格不至于太密时绘制)
    // C. 绘制网格与边界线
// 1. 计算模拟世界的总宽高（屏幕像素）
    float totalWorldW = gridW * cw;
    float totalWorldH = gridH * ch;

    // 2. 绘制外边界框 (无论缩放比例如何都显示)
    // 颜色建议：使用 cellColor 的半透明版，或者亮青色增加科技感
    ImU32 borderColor = ImGui::ColorConvertFloat4ToU32({ cellColor.x, cellColor.y, cellColor.z, 0.8f });
    draw->AddRect(
        { viewOffset.x, viewOffset.y },                             // 左上角
        { viewOffset.x + totalWorldW, viewOffset.y + totalWorldH }, // 右下角
        borderColor,
        0.0f,           // 圆角
        0,              // 边框标志
        2.0f * scale    // 线宽（随 UI 缩放，但不随 zoom 缩放，保持线条清晰）
    );

    // 3. 内部细网格线 (仅在放大时显示)
    if (zoom > 2.5f) {
        for (int x = 0; x <= gridW; ++x) {
            float posX = x * cw + viewOffset.x;
            if (posX >= 0 && posX <= winW)
                draw->AddLine({ posX, viewOffset.y }, { posX, viewOffset.y + totalWorldH }, IM_COL32(255, 255, 255, 12));
        }
        for (int y = 0; y <= gridH; ++y) {
            float posY = y * ch + viewOffset.y;
            if (posY >= 0 && posY <= winH)
                draw->AddLine({ viewOffset.x, posY }, { viewOffset.x + totalWorldW, posY }, IM_COL32(255, 255, 255, 12));
        }
    }

    // --- [3. 核心模拟逻辑] ---
    if (state.running) {
        timer += io.DeltaTime;
        totalSimTime += io.DeltaTime;
        if (timer >= tickRate) {
            timer = 0;
            generation++; // 每一帧模拟，代数增加
            for (int y = 0; y < gridH; ++y) {
                for (int x = 0; x < gridW; ++x) {
                    int neighbors = 0, idx = y * gridW + x;
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dx == 0 && dy == 0) continue;
                            neighbors += world[((y + dy + gridH) % gridH) * gridW + ((x + dx + gridW) % gridW)];
                        }
                    }
                    nextWorld[idx] = world[idx] ? (neighbors == 2 || neighbors == 3) : (neighbors == 3);
                    heatMap[idx] = nextWorld[idx] ? 1.0f : std::max(0.0f, heatMap[idx] - 0.07f);
                }
            }
            world = nextWorld;
        }
    }
    int pop = (int)std::count(world.begin(), world.end(), 1);
    history[hIdx] = (float)pop; hIdx = (hIdx + 1) % 1800;

    // 绘制细胞（光晕效果）

    ImU32 cCore = ImGui::ColorConvertFloat4ToU32(cellColor);
    ImU32 cAura = ImGui::ColorConvertFloat4ToU32({ cellColor.x, cellColor.y, cellColor.z, 0.25f });


    for (int i = 0; i < gridW * gridH; ++i) {
        if (heatMap[i] > 0.01f) {
            int ix = i % gridW;
            int iy = i / gridW;
            // 计算带偏移和缩放的坐标
            float px = ix * cw + viewOffset.x;
            float py = iy * ch + viewOffset.y;

            // 视口裁剪优化：只绘制屏幕内的细胞
            if (px + cw < 0 || px > winW || py + ch < 0 || py > winH) continue;

            draw->AddCircleFilled({ px + cw / 2, py + ch / 2 }, cw * heatMap[i] * 0.9f, cAura);
            if (world[i]) draw->AddCircleFilled({ px + cw / 2, py + ch / 2 }, cw * 0.38f, cCore);
        }
    }

    // --- [5. 交互逻辑修正] ---
    if (!io.WantCaptureMouse) {
        // 将鼠标屏幕坐标 转换为 模拟网格坐标
        // 公式：gridX = (mouse.x - offset.x) / (baseSize * zoom)
        int mx = (int)((io.MousePos.x - viewOffset.x) / cw);
        int my = (int)((io.MousePos.y - viewOffset.y) / ch);

        if (mx >= 0 && mx < gridW && my >= 0 && my < gridH) {
            if (ImGui::IsMouseDown(0)) {
                world[my * gridW + mx] = 1;
                heatMap[my * gridW + mx] = 1.0f;
            }
            if (ImGui::IsMouseDown(1)) {
                world[my * gridW + mx] = 0;
                heatMap[my * gridW + mx] = 0.0f;
            }
        }
    }

    // --- [4. 顶部状态栏优化版] ---
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)winW, 48.0f * scale });

    // 磨砂感增强：颜色稍微加深一点点，并增加细微的下边框感
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.05f, 0.04f, 0.7f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 顶栏通常不需要自带边框

    ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        // --- A. 左侧：系统标识 ---
        ImGui::SetCursorPos({ 25 * scale, 15 * scale });
        ImGui::TextColored({ 0.0f, 1.0f, 0.75f, 1.0f }, "SYS_ID: JOHN_CONWAY // GAME_OF_LIFE_CORE");

        // --- B. 中间：模拟计时器 (保持在正中央) ---
        int h = (int)totalSimTime / 3600;
        int m = ((int)totalSimTime / 60) % 60;
        int s = (int)totalSimTime % 60;

        char timeStr[32];
        sprintf(timeStr, "SIM_TIME: %02d:%02d:%02d", h, m, s);
        float timeWidth = ImGui::CalcTextSize(timeStr).x;
        // 自动居中对齐
        ImGui::SetCursorPos({ (winW - timeWidth) * 0.5f, 15 * scale });
        ImGui::TextColored(cellColor, "%s <<gen: %d // %d Units>>", timeStr, generation, pop);
        // --- C. 右侧：性能监测与控制 (靠右对齐) ---
        // 预留约 240px 的宽度给右侧区域
        float rightOffset = 240.0f * scale;
        ImGui::SetCursorPos({ (float)winW - rightOffset, 15 * scale });

        // FPS 显示
        ImGui::TextColored({ 0.4f, 0.7f, 0.5f, 0.8f }, "[ %.0f FPS ]", io.Framerate);

        // HUD 复选框
        ImGui::SameLine((float)winW - 120.0f * scale);
        ImGui::SetCursorPosY(12.0f * scale); // 复选框略微下移以对齐文字
        ImGui::PushStyleColor(ImGuiCol_CheckMark, { 0.0f, 1.0f, 0.75f, 1.0f }); // 复选框颜色与细胞同步
        ImGui::Checkbox("HUD_OSD", &showHUD);
        ImGui::PopStyleColor();
    }
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    // --- 装饰线：在顶栏下方画一条极细的极光色渐变线，增加玻璃厚度感 ---
    ImDrawList* topDraw = ImGui::GetForegroundDrawList();
    topDraw->AddLine(
        { 0, 48.0f * scale },
        { (float)winW, 48.0f * scale },
        IM_COL32(0, 255, 190, 40), 1.0f
    );

    // --- [5. 悬浮控制窗 (磨砂玻璃)] ---
    if (showHUD) {
        ImGui::SetNextWindowPos({ 40 * scale, 80 * scale }, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({ 330 * scale, 650 * scale }, ImGuiCond_FirstUseEver); // 初始大小，但可调节

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.95f, 1.0f, 0.98f, 0.18f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.35f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.2f);

        if (ImGui::Begin("LAB_CONTROLS", &showHUD)) {
            ImGui::TextColored(cellColor, "PRESET_CULTURES");
            if (ImGui::Button("GOSPER_GUN", { -1, 36 * scale })) ApplyPattern(world, gridW, gridH, 1, generation);
            if (ImGui::Button("PULSAR_STAR", { -1, 36 * scale })) ApplyPattern(world, gridW, gridH, 2, generation);
            if (ImGui::Button("STARSHIP_FLEET", { -1, 36 * scale })) ApplyPattern(world, gridW, gridH, 3, generation);

            ImGui::Separator();
            ImGui::TextColored(cellColor, "RANDOM_INIT_DENSITY");
            ImGui::SetNextItemWidth(-1); // 使滑条占满宽度
            ImGui::SliderFloat("##DensitySlider", &randomDensity, 0.01f, 0.50f, "%.2f");
            if (ImGui::Button("RANDOM_SEED", { -1, 38 * scale })) {
                std::fill(world.begin(), world.end(), (uint8_t)0);
                std::fill(heatMap.begin(), heatMap.end(), (uint8_t)0);
                for (int i = 0; i < gridW * gridH; ++i) {
                    // 使用滑条变量 randomDensity
                    world[i] = ((float)rand() / RAND_MAX < randomDensity) ? 1 : 0;
                    if (world[i]) heatMap[i] = 1.0f;
                }
                generation = 0;
            }
            ImGui::Separator();
            ImGui::TextColored(cellColor, "GRID_DIMENSIONS");

            // 使用 InputInt 或 SliderInt 编辑临时变量
            ImGui::SetNextItemWidth(120 * scale);
            ImGui::InputInt("WIDTH", &nextGridW);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120 * scale);
            ImGui::InputInt("HEIGHT", &nextGridH);

            // 限制最小值，防止为0导致崩溃
            if (nextGridW < 10) nextGridW = 10;
            if (nextGridH < 10) nextGridH = 10;
            if (nextGridW > 500) nextGridW = 500; // 建议设上限，否则性能会崩

            if (ImGui::Button("APPLY & CLEAR GRID", { -1, 30 * scale })) {
                ResizeWorld(nextGridW, nextGridH);
            }
            ImGui::Spacing();
            if (ImGui::Button("RESET_VIEW", { -1, 30 * scale })) {
                zoom = 1.0f;
                viewOffset = { 0, 0 };
            }

            ImGui::Spacing();
            ImGui::TextColored(cellColor, "ENVIRONMENT");
            ImGui::ColorEdit3("CELL_COLOR", (float*)&cellColor, ImGuiColorEditFlags_NoInputs);
            ImGui::SliderFloat("TICK_SPD", &tickRate, 0.01f, 0.5f, "%.2fs");

            ImGui::Dummy({ 0, 10 * scale });
            // 使用 cellColor 装饰读数，使其更醒目
            ImGui::Text("GENERATION : ");
            ImGui::SameLine();
            ImGui::TextColored(cellColor, "[ %d ]", generation);

            ImGui::Text("POPULATION : ");
            ImGui::SameLine();
            // 甚至可以根据人口数量加一个微型进度条或指示符
            ImGui::TextColored(cellColor, ">> %d Units", pop);

            if (ImGui::Button(state.running ? "RESUME" : "PAUSE", { -1, 40 * scale })) state.running = !state.running;

            if (ImGui::Button("RESET_ALL", { -1, 30 * scale })) {
                std::fill(world.begin(), world.end(), uint8_t(0));
                std::fill(heatMap.begin(), heatMap.end(), uint8_t(0));
                generation = 0; totalSimTime = 0;
                ResizeWorld(240, 135);
            }

            ImGui::Spacing();
            ImGui::PlotLines("##Flux", history.data(), 1800, hIdx, "BIOMASS_DENSITY", 0, gridW * gridH / 4.0f, { -1, 90 * scale });

            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 55 * scale);
            // --- 退出按钮优化：警示红磨砂效果 ---

            // 1. 设置按钮在不同状态下的红色（R, G, B, A）
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.12f, 0.12f, 0.60f)); // 常态：深红半透明
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.15f, 0.15f, 0.85f)); // 悬停：明亮红
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.00f, 0.00f, 0.00f, 1.00f)); // 点击：纯红高亮

            // 2. 如果你想让文字也变成白色（防止默认颜色看不清），可以加这一行
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));

            if (ImGui::Button("TERMINATE_LAB_SESSION", { -1, 32 * scale })) {
                state.currentScreen = AppScreen::Intro0;
                state.isIntroMode = true;
            }

            // 3. 必须弹出对应的颜色数量（这里是 4 个）
            ImGui::PopStyleColor(4);
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
    }
}

//GPU版本生命游戏
void ReallocateSimulation(GLHandles& gl, int newW, int newH) {
    // 1. 同步 GPU，确保当前没有任务在跑
    glFinish();
    cudaDeviceSynchronize();

    // 2. 彻底销毁旧资源
    if (gl.cudaRes) {
        cudaGraphicsUnregisterResource(gl.cudaRes);
        gl.cudaRes = nullptr;
    }

    // 释放 CUDA 显存并置空（防止悬空指针）
    auto SafeFree = [](void** ptr) {
        if (*ptr) { cudaFree(*ptr); *ptr = nullptr; }
        };
    SafeFree((void**)&gl.d_current);
    SafeFree((void**)&gl.d_next);
    SafeFree((void**)&gl.d_heatData);

    // 释放 OpenGL 纹理
    if (gl.lifeTex) {
        glDeleteTextures(1, &gl.lifeTex);
        gl.lifeTex = 0;
    }

    // 3. 更新尺寸
    gl.simW = newW;
    gl.simH = newH;

    // 4. 分配新显存 (注意检查错误)
    cudaMalloc(&gl.d_current, (size_t)gl.simW * gl.simH);
    cudaMalloc(&gl.d_next, (size_t)gl.simW * gl.simH);
    cudaMalloc(&gl.d_heatData, (size_t)gl.simW * gl.simH * sizeof(float));

    // 初始清零
    cudaMemset(gl.d_current, 0, (size_t)gl.simW * gl.simH);
    cudaMemset(gl.d_heatData, 0, (size_t)gl.simW * gl.simH * sizeof(float));

    // 5. 创建新 OpenGL 纹理
    glGenTextures(1, &gl.lifeTex);
    glBindTexture(GL_TEXTURE_2D, gl.lifeTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, gl.simW, gl.simH, 0, GL_RED, GL_FLOAT, nullptr);

    // 必须设置过滤，否则采样会出错
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 6. 重新建立 CUDA 映射 (互操作核心)
    cudaGraphicsGLRegisterImage(&gl.cudaRes, gl.lifeTex, GL_TEXTURE_2D, cudaGraphicsRegisterFlagsWriteDiscard);

    // 7. 重新初始化 CUDA 内部随机数状态 (重要！)
    InitCudaLife(gl.simW, gl.simH);

    // 8. 填充初始随机种子，防止黑屏
    SeedCudaLife(gl.d_current, gl.simW, gl.simH, 0.2f);

    cudaDeviceSynchronize();
}

void RenderLifeGameScreen_GPU(SimState& state, int winW, int winH, GLHandles& gl, const GpuInfo& info) {
    ImGuiIO& io = ImGui::GetIO();

    const float scale = (float)winW / 1920.0f;

    // --- [1. 模拟参数持久化] ---
    static int generation = 0;
    static float totalSimTime = 0.0f;
    static float tickTimer = 0.0f; 
    static float tickRate = 0.05f; // 步频 (秒/代)
    static int population = 0;

    
    static ImVec4 coreColor = ImVec4(0.35f, 0.98f, 0.0f, 1.0f);  // 核心绿
    static ImVec4 trailColor = ImVec4(0.85f, 0.96f, 0.8f, 1.0f); // 尾迹浅绿

    // 在 Shader 里分别传入两个 Uniform

    static float randomDensity = 0.20f;

    // 人口历史记录 (用于曲线)
    static std::vector<float> popHistory(18000, 0.0f);
    static int hIdx = 0;
    static bool showHUD = true;           // 控制面板开关

    // 1. 映射资源 (每一帧都要做)
    cudaGraphicsMapResources(1, &gl.cudaRes, 0);
    cudaGraphicsSubResourceGetMappedArray(&gl.cudaArray, gl.cudaRes, 0, 0);

    // --- [1. 定义持久化的视图变量] ---
    static float viewZoom = 1.0f;
    static ImVec2 viewOffset = { 0.0f, 0.0f };
    // --- [2. 鼠标缩放与平移逻辑] ---
    if (!io.WantCaptureMouse) {
        // A. 滚轮缩放
        if (io.MouseWheel != 0) {
            float mouseSpeed = 0.1f * viewZoom; // 缩放灵敏度随当前缩放倍数调整
            viewZoom += io.MouseWheel * mouseSpeed;
            if (viewZoom < 0.1f) viewZoom = 0.1f;   // 最小缩放
            if (viewZoom > 1000.0f) viewZoom = 1000.0f; // 最大缩放
        }
        // B. 中键平移 (Button 2)
        if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
            // 根据缩放级别调整平移速度，确保平移跟手
            viewOffset.x -= io.MouseDelta.x / (winW * viewZoom);
            viewOffset.y += io.MouseDelta.y / (winH * viewZoom); // Y翻转
        }
    }
    // --- [5. 核心修正：鼠标交互逻辑] ---
    // --- [核心修正：计算比例修正] ---
    float winAspect = (float)winW / (float)winH;
    float simAspect = (float)gl.simW / (float)gl.simH;
    ImVec2 aspectCorr = { 1.0f, 1.0f };
    if (winAspect > simAspect) {
        aspectCorr.x = winAspect / simAspect;
    }
    else {
        aspectCorr.y = simAspect / winAspect;
    }

    static int brushRadius = 1;
   // --- [鼠标交互逻辑更新] ---
    if (!io.WantCaptureMouse) {
        if (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(1)) {
            // 1. 归一化鼠标 [0, 1]
            float normX = io.MousePos.x / (float)winW;
            float normY = 1.0f - (io.MousePos.y / (float)winH);

            // 2. 逆运算 (加入 aspectCorr)
            // 注意：这里是乘法，因为在 VS 里是除法
            float worldX = (normX - 0.5f) * (aspectCorr.x / viewZoom) + 0.5f + viewOffset.x;
            float worldY = (normY - 0.5f) * (aspectCorr.y / viewZoom) + 0.5f + viewOffset.y;

            int mx = (int)(worldX * (float)gl.simW);
            int my = (int)(worldY * (float)gl.simH);

            if (mx >= 0 && mx < gl.simW && my >= 0 && my < gl.simH) {
                MousePaintCuda(gl.d_current, gl.d_heatData, gl.simW, gl.simH, mx, my, brushRadius, ImGui::IsMouseDown(1));
                // 重要：为了让交互更即时，涂抹时也同步一下 d_next，防止被下一帧逻辑覆盖
                cudaMemcpy(gl.d_next, gl.d_current, gl.simW * gl.simH, cudaMemcpyDeviceToDevice);
            }
        }
    }
    // --- [3. CUDA 计算部分] ---

    struct RuleSet { const char* name; int b; int s; };
    static RuleSet rules[] = {
        {"Conway's Life (B3/S23)",  (1 << 3), (1 << 2) | (1 << 3)},
        {"HighLife (B36/S23)",      (1 << 3) | (1 << 6), (1 << 2) | (1 << 3)},
        {"Seeds (B2/S--)",          (1 << 2), 0},
        {"WalledCities (B45678/S2345)", (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8), (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)},
        {"Amoeba (B357/S1358)",     (1 << 3) | (1 << 5) | (1 << 7), (1 << 1) | (1 << 3) | (1 << 5) | (1 << 8)}
    };
    static int currentRuleIdx = 0;
    static float trailDecay = 0.99f; // 默认衰减系数
    bool logicStepPerformed = false;

    // 1. 处理逻辑演化（只有在运行时且计时器到达时触发）
    if (state.running) {
        tickTimer += io.DeltaTime;
        totalSimTime += io.DeltaTime;

        if (tickTimer >= tickRate) {
            tickTimer = 0;
            generation++;

            // 执行逻辑计算 (paused = false)
            UpdateLifeCuda(gl.d_current, gl.d_next, gl.d_heatData, gl.simW, gl.simH, io.DeltaTime,
                false, trailDecay, rules[currentRuleIdx].b, rules[currentRuleIdx].s);

            // 交换细胞状态缓冲区
            cudaMemcpy(gl.d_current, gl.d_next, (size_t)gl.simW * gl.simH, cudaMemcpyDeviceToDevice);

            logicStepPerformed = true;

            if (generation % 10 == 0) {
                population = GetPopulationCuda(gl.d_current, gl.simW, gl.simH);
            }
        }
    }

    // 2. 【核心修复：视觉刷新】
    // 如果本帧没有进行逻辑演化（或者处于暂停状态），我们也必须更新一次热力图。
    // 这样做是为了：1. 保证尾迹平滑消失；2. 保证鼠标画上去的点立刻显示（即便暂停）。
    if (!logicStepPerformed) {
        // 仅视觉计算 (paused = true)
        UpdateLifeCuda(gl.d_current, gl.d_next, gl.d_heatData, gl.simW, gl.simH, io.DeltaTime,
            true, trailDecay, rules[currentRuleIdx].b, rules[currentRuleIdx].s);
    }
    // 更新历史曲线数据
   // 在 RenderLifeGameScreen_GPU 内部
    static float maxPopFound = 1000.0f; // 动态最大值
    static float samplingTimer = 0.0f;

    // 控制采样频率：每秒采样 60 次，无论帧率多高
    samplingTimer += io.DeltaTime;
    if (samplingTimer >= 1.0f / 60.0f) {
        samplingTimer = 0.0f;
        popHistory[hIdx] = (float)population;
        hIdx = (hIdx + 1) % 1800;

        // 动态调整 Y 轴上限：找到历史最大值，并预留 10% 的页边距
        float currentMax = 0;
        for (float v : popHistory) if (v > currentMax) currentMax = v;
        maxPopFound = std::max(maxPopFound, currentMax);
    }

    cudaMemcpy2DToArray(
        gl.cudaArray, 0, 0,
        gl.d_heatData,
        gl.simW * sizeof(float), // 这里是 Source Pitch
        gl.simW * sizeof(float), // 这里是 Width in bytes
        gl.simH,                 // 这里是 Height
        cudaMemcpyDeviceToDevice
    );
    cudaGraphicsUnmapResources(1, &gl.cudaRes, 0);

    // --- [3. 渲染背景与全屏网格] ---
    glViewport(0, 0, winW, winH);
    glUseProgram(gl.renderProg);

    // 传递缩放和平移参数
    glUniform2f(glGetUniformLocation(gl.renderProg, "winSize"), (float)winW, (float)winH);
    glUniform2f(glGetUniformLocation(gl.renderProg, "simSize"), (float)gl.simW, (float)gl.simH);
    glUniform2f(glGetUniformLocation(gl.renderProg, "viewOffset"), viewOffset.x, viewOffset.y);
    glUniform1f(glGetUniformLocation(gl.renderProg, "viewZoom"), viewZoom);

    glUniform3f(glGetUniformLocation(gl.renderProg, "coreColor"), coreColor.x, coreColor.y, coreColor.z);
    glUniform3f(glGetUniformLocation(gl.renderProg, "trailColor"), trailColor.x, trailColor.y, trailColor.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gl.lifeTex);
    glUniform1i(glGetUniformLocation(gl.renderProg, "lifeTexture"), 0);
    glBindVertexArray(gl.quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    const float barHeight = 48.0f * scale;

    // --- [4. 顶部状态栏 (专业终端风格优化)] ---
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)winW, barHeight });

    // 背景色：深色半透明，增加一点青色调
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.01f, 0.04f, 0.05f, 0.85f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10 * scale, 0));

    ImGui::Begin("TopBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar );
    {
        float centerY = 14.0f * scale; // 统一垂直居中对齐线

        // --- A. 左侧：硬件标识 ---
        ImGui::SetCursorPos({ 20 * scale, centerY });
        if (info.success) {
            ImGui::TextDisabled("HOST:"); ImGui::SameLine();
            ImGui::TextColored(coreColor, "%s", info.name);
            ImGui::SameLine();
            ImGui::TextDisabled("| CC:"); ImGui::SameLine();
            ImGui::TextColored(coreColor, "%d.%d", info.computeCapabilityMajor, info.computeCapabilityMinor);
        }
        else {
            ImGui::TextColored({ 1, 0, 0, 1 }, "CORE_OFFLINE // NO_CUDA_DETECTED");
        }

        // --- B. 中间：模拟状态 (自动居中) ---
        int h = (int)totalSimTime / 3600, m = ((int)totalSimTime / 60) % 60, s = (int)totalSimTime % 60;
        char statusStr[128];
        sprintf(statusStr, "RUNTIME: %02d:%02d:%02d  >>  GEN: %d  >>  UNITS: %d", h, m, s, generation, population);

        float textWidth = ImGui::CalcTextSize(statusStr).x;
        ImGui::SetCursorPos({ (winW - textWidth) * 0.5f, centerY });
        ImGui::TextColored(coreColor, "%s", statusStr);

        // --- C. 右侧：性能与控制 ---
        float rightSectionWidth = 260.0f * scale;
        ImGui::SetCursorPos({ (float)winW - rightSectionWidth, centerY - 2.0f * scale });

        // FPS 显示 (带平滑逻辑)
        static float displayFPS = 60.0f;
        displayFPS = displayFPS * 0.95f + io.Framerate * 0.05f;

        // 根据性能动态变色
        ImVec4 fpsColor = (displayFPS < 30.0f) ? ImVec4(1, 0.4f, 0.4f, 1) : ImVec4(0.4f, 0.8f, 0.6f, 0.8f);
        ImGui::TextColored(fpsColor, "[ %.0f FPS ]", displayFPS);

        ImGui::SameLine(0, 20 * scale);

        // HUD 切换（需要恢复 Inputs 否则 Checkbox 点不了）
        ImGui::SetCursorPosY(centerY - 5.0f * scale);
        ImGui::PushStyleColor(ImGuiCol_CheckMark, coreColor);
        // 注意：因为 TopBar 设置了 NoInputs，我们要局部开启或者单独处理 Checkbox
        // 这里建议将 TopBar 的 NoInputs 去掉，或者在 Begin 标记里删除它
        ImGui::Checkbox("HUD_OSD", &showHUD);
        ImGui::PopStyleColor();
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

    // --- 装饰线：增加厚度感 ---
    ImDrawList* topDraw = ImGui::GetForegroundDrawList();
    // 极细的发光线
    topDraw->AddLine({ 0, barHeight }, { (float)winW, barHeight }, IM_COL32(0, 255, 200, 60), 1.0f);
    // 底部微微的阴影
    topDraw->AddRectFilled({ 0, barHeight }, { (float)winW, barHeight + 4 * scale }, IM_COL32(0, 0, 0, 40));

    // --- [4. 悬浮控制窗 (可调整大小 + 磨砂玻璃)] ---
    if (showHUD) {
        // --- [5. 悬浮控制窗 (磨砂玻璃风格)] ---
        ImGui::SetNextWindowPos({ 30 * scale, 80 * scale }, ImGuiCond_FirstUseEver);
        //ImGui::SetNextWindowSize({ 340 * scale, 620 * scale });
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.95f, 1.0f, 0.98f, 0.5f));// 0.95f, 1.0f, 0.98f, 0.18f
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.2f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);

        // 静态变量用于存储 UI 输入值（避免输入时立即触发重分配）
        static int inputW = gl.simW;
        static int inputH = gl.simH;

        if (ImGui::Begin("LAB_CONTROLS", &showHUD)) {

            // A. 规则切换 (Dropdown)
            ImGui::TextColored(coreColor, "EVOLUTION_PROTOCOL");
            if (ImGui::BeginCombo("##Rules", rules[currentRuleIdx].name)) {
                for (int i = 0; i < IM_ARRAYSIZE(rules); i++) {
                    bool is_selected = (currentRuleIdx == i);
                    if (ImGui::Selectable(rules[i].name, is_selected)) {
                        currentRuleIdx = i;
                        // 切换规则时通常自动重随机一次
                        SeedCudaLife(gl.d_current, gl.simW, gl.simH, 0.2f);
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::Separator();

            // B. 视觉模式切换 (经典 vs 强化)
            static bool classicMode = false;
            if (ImGui::Checkbox("CLASSIC_VISUAL_MODE", &classicMode)) {
                if (classicMode) {
                    trailDecay = 0.0f; // 经典模式无尾迹
                    coreColor = { 0.35f, 0.98f, 0.0f, 1.0f }; // 经典绿
                }
                else {
                    trailDecay = 0.99f;
                   coreColor = ImVec4(0.35f, 0.98f, 0.0f, 1.0f);  // 核心绿
                   trailColor = ImVec4(0.85f, 0.96f, 0.8f, 1.0f); // 尾迹浅绿
                }
            }

            ImGui::TextColored(coreColor, "SYSTEM_CONFIGURATION");
            // 分辨率输入
            ImGui::InputInt("WIDTH", &inputW);
            ImGui::InputInt("HEIGHT", &inputH);

            // 限制范围防止崩溃
            if (inputW < 64) inputW = 64; if (inputH < 64) inputH = 64;
            if (inputW > 8192) inputW = 8192; if (inputH > 8192) inputH = 8192;

            // --- [新增：常用分辨率预设行] ---
            ImGui::Text("PRESETS:");
            float btnWidth = (ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().ItemSpacing.x * 4)) / 5.0f;

            if (ImGui::Button("720P", { btnWidth, 0 })) { inputW = 1280; inputH = 720; }
            ImGui::SameLine();
            if (ImGui::Button("1080P", { btnWidth, 0 })) { inputW = 1920; inputH = 1080; }
            ImGui::SameLine();
            if (ImGui::Button("2K", { btnWidth, 0 })) { inputW = 2560; inputH = 1440; }
            ImGui::SameLine();
            if (ImGui::Button("4K", { btnWidth, 0 })) { inputW = 3840; inputH = 2160; }
            ImGui::SameLine();
            if (ImGui::Button("8K", { btnWidth, 0 })) { inputW = 7680; inputH = 4320; }

            if (ImGui::Button("APPLY & REBOOT CORE", { -1, 40 })) {
                ReallocateSimulation(gl, inputW, inputH);
                viewZoom = 1.0f; viewOffset = { 0,0 };
                cudaMemcpy(gl.d_next, gl.d_current, gl.simW* gl.simH, cudaMemcpyDeviceToDevice);
            }
            // 显示当前实际尺寸
            ImGui::Text("CURRENT_GRID: %d x %d", gl.simW, gl.simH);
            ImGui::Text("TOTAL_CELLS: %d", gl.simW * gl.simH);
            // D. 性能与人口图表
            char overlay[64];
            sprintf(overlay, "CUR: %d | MAX: %.0f", population, maxPopFound);

            ImGui::PushStyleColor(ImGuiCol_PlotLines, trailColor); // 线条颜色随主题变
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0.3f)); // 背景透明

            ImGui::PlotLines("##Flux",
                popHistory.data(),
                1800,
                hIdx,
                overlay,
                0.0f,               // 最小
                maxPopFound * 1.1f, // 最大 (动态)
                ImVec2(-1, 100 * scale)
            );

            ImGui::PopStyleColor(2);

            ImGui::Text("POPULATION: %d Units", population);

            ImGui::Text("VIEW_ZOOM: %.2fx", viewZoom);
            if (ImGui::Button("RESET VIEW")) { viewZoom = 1.0f; viewOffset = { 0,0 }; }

            ImGui::Separator();
            // --- [3. UI 控制部分] ---
            
            // A. 初始化
            ImGui::TextColored(coreColor, "PRESET_BIOMASS");
            if (ImGui::Button("RANDOM_SEED", { -1, 35 * scale })) {
                SeedCudaLife(gl.d_current, gl.simW, gl.simH, randomDensity);
                generation = 0;
                totalSimTime = 0;
            }
            ImGui::SliderFloat("DENSITY", &randomDensity, 0.01f, 0.5f);

            if (ImGui::Button("RESET WORLD", { -1, 35 * scale })) {
                SeedCudaLife(gl.d_current, gl.simW, gl.simH, 0.0f);
                generation = 0;
                totalSimTime = 0;
                cudaMemcpy(gl.d_next, gl.d_current, gl.simW * gl.simH, cudaMemcpyDeviceToDevice);
            }

            ImGui::Separator();

            // B. 环境参数
            ImGui::TextColored(coreColor, "ENVIRONMENT");
            ImGui::ColorEdit3("ACTIVE_CORE", (float*)&coreColor, ImGuiColorEditFlags_NoInputs);
            ImGui::ColorEdit3("BIOMASS_TRAIL", (float*)&trailColor, ImGuiColorEditFlags_NoInputs);

            ImGui::TextColored(coreColor, "VISUAL_EFFECTS");
            // 滑块范围 0.0 (无轨迹) 到 0.99 (超长轨迹)
            ImGui::SliderFloat("TRAIL_DECAY", &trailDecay, 0.0f, 0.99f, "%.2f");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Lower = Crisper blinking\nHigher = Longer trails");

            ImGui::SliderInt("BRUSH_SIZE", &brushRadius, 1, 50);

            ImGui::SliderFloat("TICK_SPD", &tickRate, 0.001f, 0.2f, "%.3fs");

            ImGui::Checkbox("ENABLE_VSYNC", &state.vsyncEnabled);
            if (ImGui::IsItemDeactivatedAfterEdit()) glfwSwapInterval(state.vsyncEnabled ? 1 : 0);

            // C. 运行控制
            bool isPaused = !state.running;
            if (ImGui::Checkbox("PAUSE_SIMULATION", &isPaused)) state.running = !isPaused;


            // E. 退出按钮
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 55 * scale);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 0.6f));
            if (ImGui::Button("TERMINATE_SESSION", { -1, 35 * scale })) {
                state.currentScreen = AppScreen::Intro0;
            }
            ImGui::PopStyleColor();
        }

        ImGui::End();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);

    }
}

//界面4,HAM 界面设计
void RenderHamStationScreen(SimState& state, int winW, int winH) {
    float scale = (float)winW / 1920.0f;
    double t = ImGui::GetTime();
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 screenCenter(winW * 0.5f, winH * 0.5f);

    static bool showLeftPanel = true;  // 控制 STATION_INFO
    static bool showRightPanel = true; // 控制 PHONETIC_REF

    // =====================================================
    // 1. 渲染：对称钛金背景
    // =====================================================

    float subStep = 25.0f * scale, mainStep = 125.0f * scale;
    auto DrawSymmetricGrid = [&](float step, ImU32 col, bool drawDots) {
        for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
            drawList->AddLine(ImVec2(screenCenter.x + x, 0), ImVec2(screenCenter.x + x, (float)winH), col, 1.0f);
            if (x > 0) drawList->AddLine(ImVec2(screenCenter.x - x, 0), ImVec2(screenCenter.x - x, (float)winH), col, 1.0f);
        }
        for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
            drawList->AddLine(ImVec2(0, screenCenter.y + y), ImVec2((float)winW, screenCenter.y + y), col, 1.0f);
            if (y > 0) drawList->AddLine(ImVec2(0, screenCenter.y - y), ImVec2((float)winW, screenCenter.y - y), col, 1.0f);
        }
        if (drawDots) {
            for (float x = 0; screenCenter.x + x < winW || screenCenter.x - x > 0; x += step) {
                for (float y = 0; screenCenter.y + y < winH || screenCenter.y - y > 0; y += step) {
                    drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (x > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y + y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x + x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                    if (x > 0 && y > 0) drawList->AddCircleFilled(ImVec2(screenCenter.x - x, screenCenter.y - y), 3.0f * scale, IM_COL32(0, 255, 255, 35));
                }
            }
        }
        };
    DrawSymmetricGrid(subStep, IM_COL32(255, 255, 255, 5), false);
    DrawSymmetricGrid(mainStep, IM_COL32(255, 255, 255, 15), true);

    // 计算呼吸频率：周期约 3 秒，范围在 0.3 到 1.0 之间
    float morsePulse = (sinf((float)t * 2.0f) * 0.35f) + 0.65f;
    // 定义配色方案
    const ImVec4 COL_SIGNAL_WHITE = { 0.9f, 0.95f, 1.0f, 1.0f };    // 电子冷白
    const ImVec4 COL_AMBER_TUBE = { 1.0f, 0.7f, 0.1f, morsePulse }; // 呼吸琥珀金
    const ImVec4 COL_TECH_GREY = { 0.6f, 0.7f, 0.8f, 0.5f };     // 钛金辅助灰
    const ImVec4 COL_SDR_CYAN = { 0.0f, 0.9f, 1.0f, 0.8f };     // SDR 专用青

    // =====================================================
    // 0. 顶部系统工具栏 (OSD Controller)
    // =====================================================
    float toolbarH = 50.0f * scale;
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)winW, toolbarH });

    // 样式：超薄半透明钛金条
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.01f, 0.02f, 0.03f, 0.85f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    if (ImGui::Begin("##TopToolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings)) {
        
        // 绘制底部的青色装饰细线
        ImDrawList* tDraw = ImGui::GetWindowDrawList();
        tDraw->AddLine({0, toolbarH - 1}, {(float)winW, toolbarH - 1}, IM_COL32(0, 255, 255, 40), 1.0f);

        ImGui::SetCursorPos({ 20 * scale, 10 * scale });
        ImGui::TextColored(COL_SDR_CYAN, "BI9DIU // CONSOLE_OSD_v2.0");

        // --- 居中的控制按钮组 ---
        float btnW = 140.0f * scale;
        ImGui::SetCursorPos({ (winW - btnW * 2 - 10 * scale) * 0.5f, 8 * scale });

        auto DrawToggleButton = [&](const char* label, bool& state) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            
            // 根据开关状态切换颜色：激活时青色，关闭时暗灰色
            if (state) {
                ImGui::PushStyleColor(ImGuiCol_Text, COL_SIGNAL_WHITE);
                ImGui::PushStyleColor(ImGuiCol_Border, COL_SDR_CYAN);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.5f, 0.5f, 0.2f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, COL_TECH_GREY);
                ImGui::PushStyleColor(ImGuiCol_Border, COL_TECH_GREY);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 0.2f));
            }

            if (ImGui::Button(label, { btnW, 30 * scale })) {
                state = !state;
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(2);
        };

        DrawToggleButton("MONITOR_L", showLeftPanel);
        ImGui::SameLine(0, 10 * scale);
        DrawToggleButton("REFERENCE_R", showRightPanel);

        // --- 右侧系统频率显示 ---
        ImGui::SetCursorPos({ winW - 220 * scale, 12 * scale });
        ImGui::TextColored(COL_AMBER_TUBE, "OSD_FREQ: %.1f FPS", io.Framerate);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();

    // =====================================================
    // 2. 高级多层彩虹波动曲线 (SDR 频谱风格优化版)
    // =====================================================
    {
        float startX = 0, endX = (float)winW;
        float baseCenterY = winH * 0.75f;
        int numLayers = 12;
        int segments = 100; // 稍微增加段数，使颜色过渡更平滑

        for (int i = 0; i < numLayers; ++i) {
            float layerOffset = ((float)i / (float)numLayers - 0.5f) * (180.0f * scale);
            float currentY = baseCenterY + layerOffset;

            // 基础波形参数计算
            auto GetWaveY = [&](float ratio, int layerIdx) {
                float x = startX + (endX - startX) * ratio;
                // 电波干涉公式
                float wave = sinf(ratio * 8.0f + (float)t * 1.5f + (float)layerIdx * 0.4f) * (40.0f * scale) +
                    cosf(ratio * 15.0f - (float)t * 0.8f) * (15.0f * scale);

                // 鼠标靠近时波形隆起
                float dToMouse = fabsf(x - io.MousePos.x);
                if (dToMouse < 300 * scale) {
                    wave *= (1.0f + 1.5f * (1.0f - dToMouse / (300 * scale)));
                }
                return currentY + wave;
                };

            for (int j = 0; j < segments; ++j) {
                float r1 = (float)j / (float)segments;
                float r2 = (float)(j + 1) / (float)segments;

                float x1 = startX + (endX - startX) * r1;
                float x2 = startX + (endX - startX) * r2;
                float y1 = GetWaveY(r1, i);
                float y2 = GetWaveY(r2, i);

                // --- 核心修改：彩虹颜色计算 ---
                // Hue (色相) 随位置 (r1) 和 时间 (t) 变化
                // 加入 i * 0.05f 让不同层的颜色产生相位差，看起来更有深度
                float hue = fmodf(r1 + (float)t * 0.1f + i * 0.05f, 1.0f);

                // 使用 ImGui 自带的 HSV 到 RGB 转换
                ImVec4 colV4 = ImColor::HSV(hue, 0.7f, 0.9f);
                // 设置透明度：层数越靠外越透明
                colV4.w = (0.35f - i * 0.03f);
                ImU32 col = ImGui::ColorConvertFloat4ToU32(colV4);

                // 绘制主色线段
                drawList->AddLine({ x1, y1 }, { x2, y2 }, col, (1.2f + i * 0.2f) * scale);

                // 可选：增加一层极细的白色芯线，增强“高能电波”感
                drawList->AddLine({ x1, y1 }, { x2, y2 }, IM_COL32(255, 255, 255, (int)(colV4.w * 100)), 0.5f * scale);
            }
        }
    }
    // =====================================================
// 3. 左侧实时状态窗口 (色彩同步优化版)
// =====================================================

    const ImVec4 COL_STATUS_GREEN = { 0.2f, 1.0f, 0.5f, 0.4f + 0.6f * morsePulse }; // 呼吸绿

    ImGui::SetNextWindowPos({ 60 * scale, 80 * scale });
    ImGui::SetNextWindowSize({ 400 * scale, 420 * scale });

    // 样式：磨砂金属感
    

    ImGuiWindowFlags infoFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings;

    if (showLeftPanel) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.04f, 0.06f, 0.85f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
        // 边框呼吸：跟随系统节拍的青色微光
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.9f, 1.0f, 0.1f + 0.2f * morsePulse));
        if (ImGui::Begin("STATION_INFO", nullptr, infoFlags)) {
            // --- 1. 系统呼吸灯 ---
            ImGui::Spacing();
            ImGui::TextColored(COL_STATUS_GREEN, "● SYSTEM_OPERATIONAL");
            ImGui::Separator();
            ImGui::Spacing();

            // --- 2. 实时系统时间 (Signal White) ---
            std::time_t now = std::time(nullptr);
            struct tm ltm;
            localtime_s(&ltm, &now);

            ImGui::SetWindowFontScale(2.8f * scale); // 进一步加大时间显示
            ImGui::TextColored(COL_SIGNAL_WHITE, "%02d:%02d:%02d", ltm.tm_hour, ltm.tm_min, ltm.tm_sec);

            ImGui::SetWindowFontScale(1.0f * scale);
            ImGui::TextColored(COL_TECH_GREY, "UTC/LOCAL_STATION_CHRONO");
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // --- 3. UV段频率参考 (SDR Cyan & Amber) ---
            ImGui::TextColored(COL_SDR_CYAN, "[ RF_SPECTRUM_RANGE ]");
            ImGui::Spacing();

            // 标签用灰色，数值用琥珀色，模拟老式仪表感
            ImGui::TextColored(COL_TECH_GREY, "VHF BAND :");
            ImGui::SameLine(140 * scale);
            ImGui::TextColored(COL_AMBER_TUBE, "144.000 - 148.000 MHz");

            ImGui::TextColored(COL_TECH_GREY, "UHF BAND :");
            ImGui::SameLine(140 * scale);
            ImGui::TextColored(COL_AMBER_TUBE, "430.000 - 440.000 MHz");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // --- 4. 驻波比实时监控 (SWR) ---
            static float swr = 1.15f;
            swr = 1.12f + 0.04f * sinf((float)t * 4.0f);

            ImGui::TextColored(COL_SDR_CYAN, "ANT_SWR_LOAD :");
            ImGui::SameLine();
            // 数值根据状态变色，但默认保持琥珀色以同步
            ImVec4 currentSwrCol = swr < 1.5f ? COL_AMBER_TUBE : ImVec4(1, 0.2f, 0.2f, 1.0f);
            ImGui::TextColored(currentSwrCol, "%.2f", swr);

            // 进度条使用 SDR 青色
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, COL_SDR_CYAN);
            ImGui::ProgressBar((swr - 1.0f) * 1.0f, ImVec2(-1, 8 * scale), "");
            ImGui::PopStyleColor();

            // --- 5. 底部装饰标签 (与中央面板呼应) ---
            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35 * scale);
            ImGui::TextColored(COL_SIGNAL_WHITE, "OP: BI9DIU // QTH: XIAN_CHINA");
        }
        ImGui::End();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
    }

    // =====================================================
    // 4. 中央呼号与监控 (色彩美学优化版)
    // =====================================================

    ImGuiWindowFlags callsignFlags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings;

    float callsignWinWidth = 1000.0f * scale;
    ImGui::SetNextWindowPos({ screenCenter.x - callsignWinWidth * 0.5f, screenCenter.y - 180 * scale });
    ImGui::SetNextWindowSize({ callsignWinWidth, 500 * scale });

    if (ImGui::Begin("##CenterCallsign", nullptr, callsignFlags))
    {
        // 1. 主呼号：高能电子白 (Beacon Layer)
        ImGui::SetWindowFontScale(7.5f * scale);
        const char* callSignText = "BI9DIU";
        float mainTextWidth = ImGui::CalcTextSize(callSignText).x;

        // 绘制底层辉光 (增强深邃感)
        for (int i = 0; i < 2; i++) {
            drawList->AddText(NULL, 7.5f * scale * 100.0f / 100.0f,
                { screenCenter.x - mainTextWidth * 0.5f + 2.0f * scale, screenCenter.y - 145 * scale },
                IM_COL32(0, 200, 255, 15), callSignText);
        }

        ImGui::SetCursorPosX((callsignWinWidth - mainTextWidth) * 0.5f);
        ImGui::TextColored(COL_SIGNAL_WHITE, callSignText);

        // 2. 莫尔斯电码：呼吸琥珀金 (Pulse Layer)
        ImGui::SetWindowFontScale(2.0f * scale);
        const char* morseText = "-...  ..  ----.  -..  ..  ..-";
        float morseWidth = ImGui::CalcTextSize(morseText).x;

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10.0f * scale);
        ImGui::SetCursorPosX((callsignWinWidth - morseWidth) * 0.5f);
        ImGui::TextColored(COL_AMBER_TUBE, morseText);

        // 3. 字母解释法：钛金辅助灰 (Metadata Layer)
        // 采用更理性的灰色，让主信息更突出
        ImGui::SetWindowFontScale(1.15f * scale);
        const char* phoneticBreakdown = "BRAVO  INDIA  NINER  DELTA  INDIA  UNIFORM";
        float phWidth = ImGui::CalcTextSize(phoneticBreakdown).x;

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f * scale);
        ImGui::SetCursorPosX((callsignWinWidth - phWidth) * 0.5f);
        ImGui::TextColored(COL_TECH_GREY, phoneticBreakdown);

        // 4. 装饰分割线 (增加横向延伸感)
        float lineLen = 400.0f * scale;
        ImVec2 cursorLine = ImGui::GetCursorScreenPos();
        drawList->AddLine(
            { screenCenter.x - lineLen * 0.5f, cursorLine.y + 10.0f * scale },
            { screenCenter.x + lineLen * 0.5f, cursorLine.y + 10.0f * scale },
            IM_COL32(0, 255, 255, 40), 1.0f);

        // 5. 副标题：SDR 专用青 (Brand Layer)
        ImGui::SetWindowFontScale(1.8f * scale);
        const char* subText = ">> AMATEUR RADIO STATION DE XIAN";
        float subTextWidth = ImGui::CalcTextSize(subText).x;

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.0f * scale);
        ImGui::SetCursorPosX((callsignWinWidth - subTextWidth) * 0.5f);
        ImGui::TextColored(COL_SDR_CYAN, subText);
    }
    ImGui::End();
    // =====================================================
    // 5. 集中式字母解释法面板 (两列高清版)
    // =====================================================
    const char* phonetic[] = {
        "ALPHA", "BRAVO", "CHARLIE", "DELTA", "ECHO", "FOXTROT", "GOLF", "HOTEL",
        "INDIA", "JULIETT", "KILO", "LIMA", "MIKE", "NOVEMBER", "OSCAR", "PAPA",
        "QUEBEC", "ROMEO", "SIERRA", "TANGO", "UNIFORM", "VICTOR", "WHISKEY", "X-RAY",
        "YANKEE", "ZULU"
    };

    // 调整尺寸：变宽（以容纳两列），变矮（因为行数减半）
    float panelWidth = 560.0f * scale;
    float panelHeight = 520.0f * scale;
    ImGui::SetNextWindowPos({ winW - panelWidth - 60.0f * scale, 80.0f * scale });
    ImGui::SetNextWindowSize({ panelWidth, panelHeight });


    ImGuiWindowFlags refFlags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings;

    if (showRightPanel) {
        // 样式设置
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.04f, 0.06f, 0.75f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.2f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 1, 1, 0.2f));
        if (ImGui::Begin("PHONETIC_REF", nullptr, refFlags)) {
            // --- 标题居中 ---
            ImGui::SetWindowFontScale(1.0f * scale);
            float titleWidth = ImGui::CalcTextSize(" [ NATO PHONETIC REFERENCE ]").x;
            ImGui::SetCursorPosX((panelWidth - titleWidth) * 0.5f);
            ImGui::TextColored({ 0, 1, 1, 1 }, " [ NATO PHONETIC REFERENCE ]");
            ImGui::Separator();
            ImGui::Spacing();

            // --- 两列列表部分 ---
            ImGui::SetWindowFontScale(0.9f * scale); // 稍微调大一点列表字体

            float col2Offset = panelWidth * 0.5f; // 第二列的起始偏移量

            for (int i = 0; i < 13; i++) { // 循环 13行
                // --- 第一列 (A-M) ---
                int idx1 = i;
                ImGui::SetCursorPosX(25.0f * scale);
                ImGui::TextColored({ 0.0f, 1.0f, 1.0f, 1.0f }, "%c", 'A' + idx1);
                ImGui::SameLine(70.0f * scale);
                ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.85f }, "%s", phonetic[idx1]);

                // --- 第二列 (N-Z) ---
                int idx2 = i + 13;
                ImGui::SameLine(col2Offset + 20.0f * scale);
                ImGui::TextColored({ 0.0f, 1.0f, 1.0f, 1.0f }, "%c", 'A' + idx2);
                ImGui::SameLine(col2Offset + 65.0f * scale);
                ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.85f }, "%s", phonetic[idx2]);

                // 每行行间距
                ImGui::Dummy(ImVec2(0, 6.0f * scale));

                // 每隔 4 行加一个超淡的装饰线，增加理性刻度感
                if (i % 4 == 3 && i < 12) {
                    ImGui::Separator();
                    ImGui::Dummy(ImVec2(0, 2.0f * scale));
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
    }
    
    // =====================================================
// 6. 底部操作栏
// =====================================================
    float btnW = 320 * scale, btnH = 70 * scale;

    // A. 设置按钮窗口的属性
    ImGuiWindowFlags btnWindowFlags = ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings;

    // B. 设置窗口位置（对应你原来的 SetCursorPos 逻辑）
    ImGui::SetNextWindowPos({ winW - btnW - 80 * scale, winH - btnH - 80 * scale });
    ImGui::SetNextWindowSize({ btnW + 20, btnH + 20 }); // 稍微多给一点点空间

    // C. 开启窗口
    if (ImGui::Begin("##LaunchButtonWindow", nullptr, btnWindowFlags))
    {
        // 在透明窗口内部设置样式
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f * scale);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.5f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.6f, 0.8f, 0.8f));

        // 现在按钮有“家”了，Debug 窗口就会消失
        if (ImGui::Button("LAUNCH SIMULATION", { btnW, btnH })) {
            state.currentScreen = AppScreen::Intro0;
        }

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar();
    }
    ImGui::End(); // 结束按钮窗口

    // 7. 底部装饰性莫尔斯电码 (绿色呼吸灯效果)
    // =====================================================

    // 定义电台绿色 (经典荧光绿)
    ImU32 morseColor = IM_COL32(50, 255, 130, (int)(255 * morsePulse));

    // 定义背部辉光颜色 (更透、更柔和)
    ImU32 morseGlow = IM_COL32(0, 255, 100, (int)(60 * morsePulse));
    // 1. 绘制底层辉光 (稍微偏移或重复绘制以增加发光感)
    drawList->AddText(NULL, 1.2f * scale * 50.0f,
        { 80 * scale + 1.5f, winH - 60 * scale + 1.5f },
        morseGlow, "-... .. ----. -.. .. ..-");

    // 2. 绘制主体呼吸灯文字
    drawList->AddText(NULL, 1.2f * scale * 50.0f,
        { 80 * scale, winH - 60 * scale },
        morseColor, "-... .. ----. -.. .. ..-");

}

//GPU信息界面
void RenderCudaDiagnosticsScreen(SimState& state, const GpuInfo& info, int winW, int winH) {
    const float scale = (float)winW / 1920.0f;
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    // --- [1. 磨砂玻璃背景渲染] ---
    draw->AddRectFilledMultiColor({ 0,0 }, { (float)winW, (float)winH },
        IM_COL32(5, 15, 20, 255), IM_COL32(10, 30, 35, 255),
        IM_COL32(2, 10, 15, 255), IM_COL32(5, 20, 25, 255));

    draw->AddRectFilled({ 0,0 }, { (float)winW, (float)winH }, IM_COL32(100, 200, 255, 10));

    float gridSize = 64.0f * scale;
    for (float x = 0; x < winW; x += gridSize)
        draw->AddLine({ x, 0 }, { x, (float)winH }, IM_COL32(0, 255, 200, 15));
    for (float y = 0; y < winH; y += gridSize)
        draw->AddLine({ 0, y }, { (float)winW, y }, IM_COL32(0, 255, 200, 15));

    // --- [2. 顶部状态栏] ---
    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ (float)winW, 50.0f * scale });

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.05f, 0.06f, 0.8f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DiagTopBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs);
    {
        ImGui::SetCursorPos({ 20 * scale, 15 * scale });
        ImGui::TextColored({ 0.0f, 1.0f, 0.8f, 1.0f }, "CORE_DIAGNOSTICS // DEVICE_ID: %s", info.success ? "ACTIVE" : "ERROR");

        float fpsX = (float)winW - 150.0f * scale;
        ImGui::SetCursorPos({ fpsX, 15 * scale });
        ImGui::TextDisabled("[ %.0f FPS ]", io.Framerate);
    }
    ImGui::End();

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    // --- [3. 主信息面板布局] ---
    ImVec2 panelSize = { 650 * scale, 750 * scale }; // 略微加宽以适应文字
    ImGui::SetNextWindowPos({ (winW - panelSize.x) * 0.5f, (winH - panelSize.y) * 0.5f + 25 * scale });
    ImGui::SetNextWindowSize(panelSize);

    // 磨砂玻璃样式 - 准备 Push
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.15f, 0.18f, 0.65f)); // 增加不透明度提高可读性
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 0.8f, 0.3f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 15.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

    ImVec4 neonCyan = ImVec4(0.0f, 1.0f, 0.9f, 1.0f);
    ImVec4 neonGreen = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);

    if (ImGui::Begin("DIAG_PANEL", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove)) {

        ImGui::SetCursorPosY(30 * scale);
        // 安全检查：防止字体列表为空时崩溃
        if (!io.Fonts->Fonts.empty()) {
            ImGui::PushFont(io.Fonts->Fonts[0]);
        }

        ImGui::Indent(30 * scale);

        // A. 硬件标识块
        ImGui::TextColored(neonCyan, ">>> HARDWARE_IDENTITY");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("GPU_MODEL:"); ImGui::SameLine();
        ImGui::TextColored(neonGreen, "%s", info.success ? info.name : "N/A");
        ImGui::Text("ARCH_TYPE:"); ImGui::SameLine();
        ImGui::TextColored(neonGreen, "NVIDIA (SM %d.%d)", info.computeCapabilityMajor, info.computeCapabilityMinor);

        ImGui::Dummy({ 0, 25 * scale });

        // B. 计算核心规格块
        ImGui::TextColored(neonCyan, ">>> COMPUTE_SPECIFICATIONS");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "spec_cols", false);
        ImGui::SetColumnWidth(0, 280 * scale); // 宽度微调

        ImGui::Text("CUDA_CORES:");
        ImGui::TextColored(neonGreen, "%d Units", info.cudaCores);
        ImGui::Spacing();
        ImGui::Text("SM_COUNT:");
        ImGui::TextColored(neonGreen, "%d SMs", info.multiProcessorCount);

        ImGui::NextColumn();

        ImGui::Text("L2_CACHE_STATUS:");
        ImGui::TextColored(neonGreen, "DATA_LOCKED");
        ImGui::Spacing();
        ImGui::Text("CLOCK_SPEED:");
        ImGui::TextColored(neonGreen, "%.1f GHz", (float)info.clockRate / 1000000.0f);

        ImGui::Columns(1);
        ImGui::Dummy({ 0, 25 * scale });

        // C. 显存状态块
        ImGui::TextColored(neonCyan, ">>> VRAM_CAPACITY_ANALYSIS");
        ImGui::Separator();
        ImGui::Spacing();

        float vram_gb = (float)info.totalMem / (1024.0f * 1024.0f * 1024.0f);
        ImGui::Text("DEDICATED_VIDEO_MEMORY:");

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.8f, 0.7f, 0.8f));
        char vram_label[32]; sprintf(vram_label, "%.2f GB TOTAL", vram_gb);
        // 修正宽度计算：使用 -30*scale 以填满右边距
        ImGui::ProgressBar(vram_gb / 16.0f, ImVec2(-30 * scale, 28 * scale), vram_label);
        ImGui::PopStyleColor(1);

        ImGui::Dummy({ 0, 40 * scale });

        // D. 系统操作区
        ImGui::Separator();
        if (ImGui::Button("RUN_SIMULATION_STRESS_TEST", { -30 * scale, 45 * scale })) {
            // Logic for stress test
        }

        // 底部退出区域
        ImGui::SetCursorPosY(panelSize.y - 70 * scale);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.1f, 0.1f, 0.8f));
        if (ImGui::Button("TERMINATE_DIAGNOSTIC_SESSION", { -30 * scale, 35 * scale })) {
            state.currentScreen = AppScreen::Intro0;
        }
        ImGui::PopStyleColor(2);

        ImGui::Unindent();

        if (!io.Fonts->Fonts.empty()) {
            ImGui::PopFont();
        }
    }
    ImGui::End();

    // 修复：Pop 数量必须与 Push 严格对应
    ImGui::PopStyleVar(2);   // 对应 WindowRounding, WindowBorderSize
    ImGui::PopStyleColor(2); // 对应 WindowBg, Border
}

bool IsIntroScreen(AppScreen screen) {
    return (screen == AppScreen::Intro0 || screen == AppScreen::Intro1 
         || screen == AppScreen::Intro2 );
}

void RenderApp(AppScreen screen, SimState& state, const GpuInfo& info, int winW, int winH, GLHandles& gl, AsyncLoader& loader) {

    switch (screen) {
    case AppScreen::Intro0:
        RenderIntroScreen0(state, winW, winH, state.isIntroMode, loader);
        break;
    case AppScreen::Intro1:
        RenderIntroScreen1(state, winW, winH, state.isIntroMode, loader);
        break;
    case AppScreen::Intro2:
        RenderIntroScreen2(state, winW, winH, state.isIntroMode, loader);
        break;
    case AppScreen::LifeGame:
        RenderLifeGameScreen(state, winW, winH);
        break;
    case AppScreen::LifeGame2:
        RenderLifeGameScreen_GPU(state, winW, winH,gl, info);
        break;
    case AppScreen::HamStation:
        RenderHamStationScreen(state, winW, winH);
        break;
    case AppScreen::CudaDiagno:
        RenderCudaDiagnosticsScreen(state, info,winW, winH);
        break;
    
    default: break;
    }
}

void CycleScreens(SimState& state) {
    // 自动根据枚举数量轮换，无需手动修改数字
    int total = static_cast<int>(AppScreen::Count); // 假设你在末尾加了 Count
    int next = (static_cast<int>(state.currentScreen) + 1) % total;
    state.currentScreen = static_cast<AppScreen>(next);

    // 这里的逻辑只负责跳转，不负责改变其他状态
    std::cout << "Switched to screen index: " << next << std::endl;
}
//检查是否有操作
void CheckIdleStatus(GLFWwindow* window, SimState& state) {
    double currentTime = glfwGetTime();
    bool inputDetected = false;

    // 1. 检查鼠标移动
    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    if (abs(mx - state.lastMouseX) > 1.0 || abs(my - state.lastMouseY) > 1.0) {
        inputDetected = true;
        state.lastMouseX = mx;
        state.lastMouseY = my;
    }

    // 2. 检查鼠标点击
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        inputDetected = true;
    }

    // 3. 检查键盘输入 (简单检查几个常用键，或者使用 GLFW 回调更全面)
    // 这里简单检查 ImGui 是否捕获了键盘，或者是否有任意键按下
    // 注意：ImGui::GetIO().WantCaptureKeyboard 只能说明 ImGui 在处理，也算一种操作
    if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse) {
        inputDetected = true;
    }

    // 4. 更新状态
    if (inputDetected) {
        state.lastInputTime = currentTime;
    }
    else {
        // 如果正在运行模拟，且超过了空闲时间
        if (!state.isIntroMode && (currentTime - state.lastInputTime > state.idleTimeout)) {
            state.isIntroMode = true; // 自动进入屏保/欢迎页
            state.running = false;    // 可选：暂停模拟以省电
            //state.RenderVisualization = false; // 可选：关闭渲染
            std::cout << "[System] Idle timeout reached. Returning to Intro Screen." << std::endl;
        }
    }
}

//资源清理
void cleanup(GLHandles& gl) {
    // --- 1. 注销 CUDA 与 OpenGL 的互操作资源 ---
    if (gl.cudaRes) {
        // 在删除 OpenGL 纹理之前，必须先注销注册的资源
        cudaGraphicsUnregisterResource(gl.cudaRes);
        gl.cudaRes = nullptr;
    }

    // --- 2. 释放 CUDA 申请的逻辑缓冲区 (显存) ---
    if (gl.d_current) {
        cudaFree(gl.d_current);
        gl.d_current = nullptr;
    }
    if (gl.d_next) {
        cudaFree(gl.d_next);
        gl.d_next = nullptr;
    }
    if (gl.d_heatData) {
        cudaFree(gl.d_heatData);
        gl.d_heatData = nullptr;
    }

    // --- 3. 清理 OpenGL 纹理 ---
    if (gl.lifeTex) {
        glDeleteTextures(1, &gl.lifeTex);
        gl.lifeTex = 0;
    }

    // --- 4. 清理基础几何资源 (原有逻辑) ---
    if (gl.quadVBO) {
        glDeleteBuffers(1, &gl.quadVBO);
        gl.quadVBO = 0;
    }
    if (gl.quadVAO) {
        glDeleteVertexArrays(1, &gl.quadVAO);
        gl.quadVAO = 0;
    }

    // --- 5. 清理着色器程序 (原有逻辑) ---
    if (gl.renderProg) {
        glDeleteProgram(gl.renderProg);
        gl.renderProg = 0;
    }

    // 重置 CUDA 数组指针（它是通过映射获取的，不需要单独释放）
    gl.cudaArray = nullptr;
}

//处理键盘操作
void ProcessInput(GLFWwindow* window, SimConfig& cfg, SimState& state, GLHandles& gl) {
    if (!ImGui::GetIO().WantTextInput) {
        if (ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
            CycleScreens(state);
        }
    }

    // --- 空格键切换运行状态 ---
    static bool spaceWasPressed = false; // 记录上一帧空格键是否按下
    bool spaceIsPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

    // 逻辑：当前按下且上一帧没按 -> 触发切换
    if (spaceIsPressed && !spaceWasPressed) {
        state.running = !state.running;
    }
    spaceWasPressed = spaceIsPressed; // 更新状态
}
