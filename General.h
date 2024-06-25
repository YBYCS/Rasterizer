//
// Created by azuslai on 22/5/2024.
//
#ifndef RASTERIZER_GENERAL_H
#define RASTERIZER_GENERAL_H

#include <cstdint>
#include "Vector.h"
#include "Color.h"

//顶点的数据结构
struct VertexData {
    VertexData() = default;
    VertexData(const Vector4& position, const Vector4& color, const Vector3& normal, const Vector2& texCoord) : position(position), color(color), normal(normal), texCoord(texCoord) {}
    //一开始从顶点着色器输出后表示裁剪空间位置，经历透视除法后表示NDC空间位置，经过屏幕空间变换后又表示屏幕空间位置
    Vector4 position = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    //在着色器语言(GLSL、HLSL)中，颜色通常被当作向量处理
    //表达颜色的向量用 0.0f-1.0f 映射 0-255
    //例如：Vector4(1.0f, 0.0f, 0.0f, 1.0f) 表示纯红色
    Vector4 color;        
    Vector3 normal;       //法线
    Vector2 texCoord;     //纹理坐标
    float oneOverW = 0.0f;       //记录剪裁空间下 1 / position，用于透视修正
};

//片元着色器输出结构
struct FragmentShaderOutput {
    Vector2 position;   //像素的屏幕空间的位置
    float depth;    //像素深度
    Color color;    //最终颜色
};

// 将 Color 转换为 Vector4
inline Vector4 ColorToVector4(const Color& color) { return Vector4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f); };
// 将 Vector4 转换为 Color
inline Color Vector4ToColor(const Vector4& v) { return Color(v.x * 255.0f, v.y * 255.0f, v.z * 255.0f, v.w * 255.0f); };

class General {

};

#endif //RASTERIZER_GENERAL_H