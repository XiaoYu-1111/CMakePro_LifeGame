#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D lifeTexture;
uniform vec3 coreColor;   // C++ 传入的核心颜色 (Active)
uniform vec3 trailColor;  // C++ 传入的尾迹颜色 (Biomass)
uniform vec2 simSize;     // 【此时该尺寸已在 C++ 端完成了旋转宽高的自动交换】
uniform float viewZoom;
uniform float totalTime;

// 旋转与镜像控制
uniform bool rotate90;    
uniform bool flipX;       
uniform bool flipY;       

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
    // 1. 坐标方向映射
    vec2 uv = TexCoords;
    if (rotate90) {
        uv = vec2(uv.y, uv.x); // U、V 轴对调
    }
    if (flipX) {
        uv.x = 1.0 - uv.x;     // 水平翻转
    }
    if (flipY) {
        uv.y = 1.0 - uv.y;     // 垂直翻转
    }

    // 采样热力值
    float heat = texture(lifeTexture, uv).r;
    
    // 1. 基础背景
    vec3 baseBG = vec3(0.01, 0.05, 0.06); 
    
    // 2. 计算动态网格 (直接使用已在 C++ 交换好的 simSize)
    float thickness = mix(0.3, 0.06, clamp(viewZoom * 0.1, 0.0, 1.0));
    float grid = getGrid(uv, simSize, thickness);
    float gridFinal = grid * mix(0.04, 0.12, clamp(viewZoom * 0.2, 0.0, 1.0));

    // --- [核心颜色分层逻辑] ---
    vec3 finalColor = baseBG;

    // A. 计算尾迹层
    vec3 trailLayer = trailColor * pow(heat, 1.5);
    finalColor = mix(finalColor, trailLayer, smoothstep(0.0, 0.6, heat));

    // B. 计算核心层
    float coreFactor = smoothstep(0.9, 0.99, heat);
    vec3 coreLayer = coreColor * 1.2; 
    finalColor = mix(finalColor, coreLayer, coreFactor);

    // C. 叠加发光增强
    if(heat > 0.8) {
        finalColor += coreColor * (heat - 0.8) * 0.5;
    }

    // 3. 叠加网格 
    finalColor += trailColor * gridFinal * 1.5;

    // --- [绘制内部边界白线，直接使用已在 C++ 交换好的 simSize] ---
    vec2 cellPos = uv * simSize;
    
    float dLeft   = abs(cellPos.x - 1.0);
    float dRight  = abs(cellPos.x - (simSize.x - 1.0));
    float dBottom = abs(cellPos.y - 1.0);
    float dTop    = abs(cellPos.y - (simSize.y - 1.0));

    float distToBorder = 1e6;
    if(cellPos.x >= 0.9 && cellPos.x <= simSize.x - 0.9) {
        distToBorder = min(distToBorder, dBottom);
        distToBorder = min(distToBorder, dTop);
    }
    if(cellPos.y >= 0.9 && cellPos.y <= simSize.y - 0.9) {
        distToBorder = min(distToBorder, dLeft);
        distToBorder = min(distToBorder, dRight);
    }

    // 线条抗锯齿绘制
    float borderLineThickness = 1.5; 
    float borderAA = fwidth(distToBorder);
    float whiteBorder = smoothstep(borderAA * borderLineThickness, 0.0, distToBorder - 0.05);
    
    // 叠加白线边界
    finalColor = mix(finalColor, vec3(1.0, 1.0, 1.0), whiteBorder * 0.8); 

    // 4. 后期：扫描线 (基于屏幕空间 Y 轴，保持完美的水平状态)
    float scanline = sin(TexCoords.y * 1080.0 + totalTime * 4.0) * 0.01;
    finalColor += scanline;

    // 5. 后期：暗角 (基于屏幕空间中心)
    float vignette = distance(TexCoords, vec2(0.5));
    finalColor *= smoothstep(1.3, 0.45, vignette);

    // 6. 输出结果
    FragColor = vec4(finalColor, 0.8); 
}