#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D lifeTexture;
uniform vec3 coreColor;   // C++ 传入的核心颜色 (Active)
uniform vec3 trailColor;  // C++ 传入的尾迹颜色 (Biomass)
uniform vec2 simSize;   
uniform float viewZoom;
uniform float totalTime;

// 抗锯齿网格函数
float getGrid(vec2 uv, vec2 size, float thickness) {
    vec2 gridPos = uv * size;
    vec2 f = fract(gridPos);
    vec2 df = fwidth(gridPos);
    vec2 g = smoothstep(df * (thickness + 0.5), df * (thickness - 0.5), f) + 
             smoothstep(1.0 - df * (thickness - 0.5), 1.0 - df * (thickness + 0.5), f);
    return max(g.x, g.y);
}

void main() {
    // 采样热力值 (0.0 = 死亡, 1.0 = 刚活/核心)
    float heat = texture(lifeTexture, TexCoords).r;
    
    // 1. 基础背景 (深色实验室质感)
    vec3 baseBG = vec3(0.01, 0.05, 0.06); 
    
    // 2. 计算动态网格 (随缩放变化)
    float thickness = mix(0.3, 0.06, clamp(viewZoom * 0.1, 0.0, 1.0));
    float grid = getGrid(TexCoords, simSize, thickness);
    float gridFinal = grid * mix(0.04, 0.12, clamp(viewZoom * 0.2, 0.0, 1.0));

    // --- [核心颜色分层逻辑] ---
    
    vec3 finalColor = baseBG;

    // A. 计算尾迹层 (Trail Layer)
    // 使用非线性衰减 pow(heat, 1.5) 让尾迹末端消失得更自然
    vec3 trailLayer = trailColor * pow(heat, 1.5);
    // 只有热力值大于 0 的地方才混合尾迹
    finalColor = mix(finalColor, trailLayer, smoothstep(0.0, 0.6, heat));

    // B. 计算核心层 (Core Layer)
    // 当 heat 接近 1.0 时，平滑切换到 coreColor
    // 使用 smoothstep(0.9, 1.0) 确保只有核心最亮的部分显示 coreColor
    float coreFactor = smoothstep(0.9, 0.99, heat);
    vec3 coreLayer = coreColor * 1.2; // 核心稍微过载增亮
    finalColor = mix(finalColor, coreLayer, coreFactor);

    // C. 叠加发光增强 (Glow/Bloom Effect)
    // 让核心颜色对周围产生一点光晕影响
    if(heat > 0.8) {
        finalColor += coreColor * (heat - 0.8) * 0.5;
    }

    // 3. 叠加网格 (使用尾迹颜色作为网格的荧光色，保持色调统一)
    finalColor += trailColor * gridFinal * 1.5;

    // --- [新增：绘制内部边界白线] ---
    // 将纹理坐标映射到实际网格单位 (0 到 simSize)
    vec2 cellPos = TexCoords * simSize;
    
    // 计算当前点距离“向内1格”边界的距离
    // 目标边界线位置：x=1, y=1, x=simSize.x-1, y=simSize.y-1
    float dLeft   = abs(cellPos.x - 1.0);
    float dRight  = abs(cellPos.x - (simSize.x - 1.0));
    float dBottom = abs(cellPos.y - 1.0);
    float dTop    = abs(cellPos.y - (simSize.y - 1.0));

    // 获取四条边中最短的距离
    float distToBorder = 1e6;
    // 只有在矩形框范围内的像素才参与计算，防止线条延伸到无穷远
    if(cellPos.x >= 0.9 && cellPos.x <= simSize.x - 0.9) {
        distToBorder = min(distToBorder, dBottom);
        distToBorder = min(distToBorder, dTop);
    }
    if(cellPos.y >= 0.9 && cellPos.y <= simSize.y - 0.9) {
        distToBorder = min(distToBorder, dLeft);
        distToBorder = min(distToBorder, dRight);
    }

    // 线条粗细 (像素单位，可根据需要调整)
    float borderLineThickness = 1.5; 
    // 使用 fwidth 实现屏幕空间的抗锯齿
    float borderAA = fwidth(distToBorder);
    float whiteBorder = smoothstep(borderAA * borderLineThickness, 0.0, distToBorder - 0.05);
    
    // 只有在视野缩放比较近的时候才显示白线，或者全程显示
    finalColor = mix(finalColor, vec3(1.0, 1.0, 1.0), whiteBorder * 0.8); 


    // 4. 后期：扫描线 (Scanlines)
    float scanline = sin(TexCoords.y * 1080.0 + totalTime * 4.0) * 0.01;
    finalColor += scanline;

    // 5. 后期：暗角 (Vignette)
    float vignette = distance(TexCoords, vec2(0.5));
    finalColor *= smoothstep(1.3, 0.45, vignette);

    // 6. 输出结果 (Alpha 为 0.8 方便与底层 ImDrawList 混合)
    FragColor = vec4(finalColor, 0.8); 
}