#pragma once

#include "main.h"
#include "Point.h"
#include "Image.h"
#include "General.h"
#include <vector>

enum class SamplingMethod {
    NEAREST_NEIGHBOR,   //邻近过滤采样
    BILINEAR_INTERPOLATION, //双线性插值采样
};

enum class TextureWarpMode {
    REPEAT, //重复
    MIRRORED_REPEAT,    //镜像重复
    CLAMP,  //限制
};

enum class DrawMode {
    LINE,       //绘制直线
    TRIANGLE,   //绘制三角形
};

class Rasterizer
{
private:
    static SamplingMethod samplingMethod_; //采样方式
    static TextureWarpMode textureWarpMode_;    //纹理warp模式
private:
    Rasterizer();
    ~Rasterizer();
    static float GetTriangleArea(const Point &p1, const Point &p2, const Point &p3);
    static float GetTriangleArea(const Vector2& p1, const Vector2& p2, const Vector2& p3);
    static Color SampleTextureNearest(const Vector2 &uv, const Image *texture = nullptr);
    static Color SampleTextureBilinear(const Vector2 &uv, const Image *texture = nullptr);
    static void WrapUV(Vector2& uv);
    //光栅化一个线段
    static void RasterizeLine(std::vector<VertexData> &output, const VertexData& v1, const VertexData& v2);
    //光栅化一个三角形
    static void RasterizeTriangle(std::vector<VertexData> &output, const VertexData& v1, const VertexData& v2, const VertexData& v3, const Image* texture = nullptr);
public:
    static void SetSamplingMethod(SamplingMethod method);
    static SamplingMethod GetSamplingMethod();
    static void SetTextureWarpMode(TextureWarpMode mode);
    static TextureWarpMode GetTextureWarpMode();
    //从纹理中采样颜色
    static Color Sampling(Vector2 texCoord, const Image *texture);
    static void DrawImage(const Image* image);
    static void DrawImageWithAlpha(const Image* image, byte alpha);

    //将顶点着色器的输出转换为屏幕上的像素
    static std::vector<VertexData> Rasterize(DrawMode drawMode, const std::vector<VertexData>& input, const Image* texture = nullptr);
};