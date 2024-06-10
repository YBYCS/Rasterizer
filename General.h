//
// Created by azuslai on 22/5/2024.
//
#ifndef RASTERIZER_GENERAL_H
#define RASTERIZER_GENERAL_H

#include <cstdint>
#include "Vector.h"
#include "Color.h"

struct VertexAttributeDescriptor {
    uint32_t vboId;
    size_t size;           //组件数量
    size_t stride;         //步长
    size_t offset;         //偏移量
};

//顶点着色器输出的每一个顶点的数据结构
struct VertexOutput {
    //一开始从顶点着色器输出后表示裁剪空间位置，经历透视除法后表示NDC空间位置，经过屏幕空间变换后又表示屏幕空间位置
    Vector4 position = { 0.0f, 0.0f, 0.0f, 1.0f }; 
    //表达颜色的向量用 0.0f-1.0f 映射 0-255
    //例如：Vector4(1.0f, 0.0f, 0.0f, 1.0f) 表示纯红色
    Vector4 color;        
    Vector2 texCoord;     //纹理坐标
};

//片元着色器输出结构
struct FragmentOutput {
    Vector2 position;   //像素的屏幕空间的位置
    float depth;    //像素深度
    Color color;    //最终颜色
};

class General {

};

#endif //RASTERIZER_GENERAL_H