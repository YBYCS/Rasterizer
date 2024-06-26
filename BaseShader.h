#pragma once

#include "General.h"
#include "Point.h"
#include "Matrix.h"
#include "Image.h"

/// @brief 平行光
struct DirectionalLight {
    Vector3 direction; //光源方向
    Vector3 color;     //光源颜色
    float intensity;   //光照强度
};
/// @brief 环境光
struct AmbientLight {
    Vector3 color;     //光源颜色
    float intensity;   //光照强度
};

class BaseShader {
protected:
    Matrix4 modelMatrix_;
    Matrix4 viewMatrix_;
    Matrix4 projectionMatrix_;
public:
    DirectionalLight directionalLight;
    AmbientLight ambientLight;   //环境光
public:
    BaseShader() = default;
    virtual ~BaseShader() = default;

    virtual VertexData VertexShader(const VertexData& input) = 0;
    virtual bool FragmentShader(const VertexData& input, FragmentShaderOutput &output, const Image *texture = nullptr) = 0;

    virtual void SetModelMatrix(const Matrix4 &modelMatrix);
    virtual void SetViewMatrix(const Matrix4 &viewMatrix);
    virtual void SetProjectMatrix(const Matrix4 &projectMatrix);
};