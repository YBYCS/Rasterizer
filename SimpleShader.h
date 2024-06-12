#pragma once

#include "BaseShader.h"
#include "Matrix.h"

//此简易shader只应用MVP变换，不应用光照
class Simpleshader : public BaseShader {
public:
    Simpleshader() = default;
    virtual ~Simpleshader() = default;

    VertexData VertexShader(const VertexData &input) override;
    bool FragmentShader(const VertexData& input, FragmentShaderOutput &output) override;  
};