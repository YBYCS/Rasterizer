#include "BaseShader.h"

void BaseShader::SetModelMatrix(const Matrix4 &modelMatrix)
{
    modelMatrix_ = modelMatrix;
}

void BaseShader::SetViewMatrix(const Matrix4 &viewMatrix)
{
    viewMatrix_ = viewMatrix;
}

void BaseShader::SetProjectMatrix(const Matrix4 &projectionMatrix)
{
    projectionMatrix_ = projectionMatrix;
}
