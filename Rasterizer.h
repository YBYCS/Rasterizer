#pragma once

#include "main.h"
#include "Point.h"
#include "Image.h"

enum SamplingMethod {
    NEAREST_NEIGHBOR,   //邻近过滤采样
    BILINEAR_INTERPOLATION, //双线性插值采样
};

enum TextureWarpMode {
    REPEAT, //重复
    MIRRORED_REPEAT,    //镜像重复
    CLAMP,  //限制
};

class Rasterizer
{
private:
    static bool enableBlending; //是否允许颜色混合
    static SamplingMethod samplingMethod_; //采样方式
    static TextureWarpMode textureWarpMode_;    //纹理warp模式
    static Image* texture_;
private:
    Rasterizer();
    ~Rasterizer();
    static float GetTriangleArea(const Point &p1, const Point &p2, const Point &p3);
    static Color SampleTextureNearest(const Vector2 &uv);
    static Color SampleTextureBilinear(const Vector2 &uv);
    static void WrapUV(Vector2& uv);
public:
    static void SetBlending(bool enabled);
    static bool IsBlendingEnabled();
    static void SetSamplingMethod(SamplingMethod method);
    static SamplingMethod GetSamplingMethod();
    static void SetTextureWarpMode(TextureWarpMode mode);
    static TextureWarpMode GetTextureWarpMode();
    static void DrawLine(const Point& p1, const Point& p2);
    static void DrawTriangle(const Point& p1, const Point& p2, const Point& p3);
    static void DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3);
    static void DrawImage(const Image* image);
    static void DrawImageWithAlpha(const Image* image, byte alpha);
    static void SetTexture(Image* texture);
};