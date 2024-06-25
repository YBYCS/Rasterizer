#pragma once

#include "BaseShader.h"
#include "Model.h"
#include "General.h"
#include "Matrix.h"
#include "Image.h"
#include "Rasterizer.h"

class Render {
private:
    static BaseShader* curShader_;
    static Matrix4 screenMatrix_;
    static bool enableBlending_; //是否允许颜色混合
    static float *depthMap_;    //深度图
private:
    //使用顶点着色器处理顶点数据
    static VertexData VertexShader(const Vector3& position, const Color& color, const Vector3& normal, const Vector2& texCoord);
    //透视除法，用于将顶点从剪裁空间转换到NDC空间
    static void PerspectiveDivision(std::vector<VertexData> &output);
    //恢复透视修正
    static void PerspectiveCorrection(std::vector<VertexData> &output);
    //视口变换
    static void ViewPortTransform(std::vector<VertexData> &output);
    static bool DepthTest(const FragmentShaderOutput &output);
    //背面剔除
    static bool BackfaceCulling(const VertexData &v0, const VertexData &v1, const VertexData &v2);
    //耳剪法，将多边形拆分成三角形
    static void EarClipping(std::vector<VertexData> &res, const Model &model, const Face& targetFace);
    //在剪裁空间下进行一系列剪裁操作
    static void ClipInClipSpace(const DrawMode &drawMode, const std::vector<VertexData> &input, std::vector<VertexData> &output);
    //剪裁算法
    static void Sutherland_Hodgman(const DrawMode &drawMode, const std::vector<VertexData> &input, std::vector<VertexData> &output);
public:
    static void SetBlending(bool enabled);
    static bool IsBlendingEnabled();
    //选择使用哪一个shader
    static void SetShader(BaseShader *shader);
    //初始化深度图
    static void InitializeDepthMap(float value = 1.0f);
    static void RenderModel(const Model& model, Image* textureImage);
};