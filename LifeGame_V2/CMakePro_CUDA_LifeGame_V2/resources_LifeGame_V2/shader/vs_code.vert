#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform vec2 viewOffset;
uniform float viewZoom;
uniform vec2 winSize; // 窗口宽高
uniform vec2 simSize; // 模拟网格宽高

void main() {
    gl_Position = vec4(aPos, 1.0);

    // 计算缩放修正系数
    float winAspect = winSize.x / winSize.y;
    float simAspect = simSize.x / simSize.y;
    
    // 让模拟画面始终保持原比例，居中显示
    vec2 aspectCorrection = vec2(1.0, 1.0);
    if (winAspect > simAspect) {
        aspectCorrection.x = winAspect / simAspect;
    } else {
        aspectCorrection.y = simAspect / winAspect;
    }

    // 应用修正后的缩放和平移
    TexCoords = (aTexCoord - 0.5) * aspectCorrection / viewZoom + 0.5 + viewOffset;
}