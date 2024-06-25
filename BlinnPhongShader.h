#pragma once

#include "BaseShader.h"
#include "Vector.h"

class BlinnPhongShader : public BaseShader {
public:
    DirectionalLight directionalLight;
    AmbientLight ambientLight;   //环境光

public:
    BlinnPhongShader() = default;
    virtual ~BlinnPhongShader() = default;

    VertexData VertexShader(const VertexData &input) override;
    bool FragmentShader(const VertexData& input, FragmentShaderOutput &output, const Image *image = nullptr) override;
};
