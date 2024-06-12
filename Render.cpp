#include "Render.h"
#include "Rasterizer.h"
#include <iostream>
#include "WindowController.h"

BaseShader* Render::curShader_ = nullptr;
Matrix4 Render::screenMatrix_ = ScreenMatrix();
bool Render::enableBlending_ = false;
float* Render::depthMap_ = new float[window->GetWidth() * window->GetHeight()];

VertexData Render::VertexShader(const Vector3 &position, const Color &color, const Vector3 &normal, const Vector2 &texCoord)
{
    VertexData vertex;
    vertex.position = Vector4(position.x, position.y, position.z, 1.0f);
    vertex.color = ColorToVector4(color);
    vertex.normal = normal;
    vertex.texCoord = texCoord;

    return curShader_->VertexShader(vertex);
}

void Render::PerspectiveDivision(std::vector<VertexData> &output)
{
    for (auto &vertexData : output) {
        vertexData.position *= 1.0f / vertexData.position.w;
        vertexData.position.w = 1.0f;
    }
}

void Render::ViewPortTransform(std::vector<VertexData> &output)
{
    for (auto &vertexData : output) {
        vertexData.position = screenMatrix_ * vertexData.position;
    }
}

bool Render::DepthTest(const FragmentShaderOutput &output)
{
    uint32_t index = output.position.y * window->GetWidth() + output.position.x;
    float oldDepth = depthMap_[index];
    if (output.depth < oldDepth) {
        depthMap_[index] = output.depth;
        return true;
    }
    return false;
}

bool Render::BackfaceCulling(const VertexData &v0, const VertexData &v1, const VertexData &v2)
{
    //此时处于NDC空间下，NDC坐标系是左手坐标系！
    //因此背面剔除就是剔除掉法线的z值大于0的的三角形
    auto p1 = v1.position - v0.position;
    Vector3 edge1(p1.x, p1.y);
    auto p2 = v2.position - v0.position;
    Vector3 edge2(p2.x, p2.y);

    return Cross(edge1, edge2).z > 0;
}

void Render::SetBlending(bool enabled)
{
    enableBlending_ = enabled;
}

bool Render::IsBlendingEnabled()
{
    return enableBlending_;
}

void Render::SetShader(BaseShader *shader)
{
    curShader_ = shader;
}

void Render::InitializeDepthMap(float value)
{
    std::fill_n(depthMap_, window->GetWidth() * window->GetHeight(), value);
}

void Render::RenderModel(const Model &model, Image *textureImage)
{
    if (curShader_ == nullptr) {
        std::__throw_runtime_error("shader is null !");
    }

    //首先将模型的所有顶点经过 VertexShader 处理后，装入到一个vector中
    //在这一步就已经进行完了MVP变换
    std::vector<VertexData> vertexesData;

    for (uint32_t i = 0; i < model.faces.size(); i++) {
        auto face = model.faces[i];
        for (int j = 0; j < face.vertexIndices.size(); j++) {
            Vector3 position = model.vertices[face.vertexIndices[j]];
            Vector3 normal = model.normals[face.normalIndices[j]];
            Vector2 uv = model.uvs[face.uvIndices[j]];
            //obj文件只用纹理获取颜色，默认底色为白色
            vertexesData.emplace_back(VertexShader(position, Color(), normal, uv));
        }
    }
    
    if (vertexesData.empty()) {
        std::__throw_runtime_error("model is null !");
    }
    //Todo:裁剪空间进行裁剪

    //将裁剪处理后的顶点数据，从剪裁空间转换到NDC空间
    PerspectiveDivision(vertexesData);

    //背面剔除
    std::vector<VertexData> collOutputs;
    for (uint32_t i = 0; i < vertexesData.size() - 2; i += 3) {
        if (BackfaceCulling(vertexesData[i], vertexesData[i + 1], vertexesData[i + 2])) {
            collOutputs.insert(collOutputs.end(), vertexesData.begin() + i, vertexesData.begin() + i + 3);
        }
    }

    //应用视口变换映射到屏幕空间
    ViewPortTransform(collOutputs);

    //光栅化处理
    std::vector<VertexData> rasterOutputs = Rasterizer::Rasterize(DrawMode::TRIANGLE, collOutputs, textureImage);
    if (rasterOutputs.empty())
        return;
        
    //Todo:透视修正

    //应用 FragmentShader 
    FragmentShaderOutput fsOutput;
    for (uint32_t i = 0; i < rasterOutputs.size(); ++i) {
        curShader_->FragmentShader(rasterOutputs[i], fsOutput);
        //深度测试
        if (!DepthTest(fsOutput)) 
            continue;
        window->DrawPoint(fsOutput.position.x, fsOutput.position.y, fsOutput.color);
    }
}