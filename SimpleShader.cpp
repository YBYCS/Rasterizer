#include "SimpleShader.h"

VertexData Simpleshader::VertexShader(const VertexData &input)
{
    //此简易shader只应用MVP变换，不应用光照
    auto pos = input.position;
    Vector4 curPosition(pos.x, pos.y, pos.z, 1.0f);
    curPosition = projectionMatrix_ * viewMatrix_ * modelMatrix_ * curPosition;
    //变换法线向量
    Vector4 curNormal(input.normal.x, input.normal.y, input.normal.z, 0.0f); 
    curNormal = Transpose(Inverse(modelMatrix_)) * curNormal; //计算模型矩阵的逆转置矩阵
    Normalize(curNormal);
    return VertexData(curPosition, input.color, Vector3(curNormal.x, curNormal.y, curNormal.z), input.texCoord);
}

bool Simpleshader::FragmentShader(const VertexData &input, FragmentShaderOutput &output)
{
    output.position = input.position;
    output.depth = input.position.z;
    output.color = Vector4ToColor(input.color);
    return true;
}