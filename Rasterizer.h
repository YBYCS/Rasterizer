#pragma once

#include "main.h"
#include "Point.h"
#include "Image.h"

class Rasterizer
{
private:
    static bool enableBlending; //是否允许颜色混合
    static bool enableBilinearSampling; //是否启用双线性插值采样，否则使用邻近过滤采样  
    static Image* texture_;
private:
    Rasterizer();
    ~Rasterizer();
    static float GetTriangleArea(const Point &p1, const Point &p2, const Point &p3);
    static Color SampleTextureNearest(const Vector2 &uv);
    static Color SampleTextureBilinear(const Vector2 &uv);
public:
    static void SetBlending(bool enabled);
    static void SetBilinearInterpolationEnabled(bool enabled);
    static bool IsBlendingEnabled();
    static bool IsBilinearSamplingEnabled();
    static void DrawLine(const Point& p1, const Point& p2);
    static void DrawTriangle(const Point& p1, const Point& p2, const Point& p3);
    static void DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3);
    static void DrawImage(const Image* image);
    static void DrawImageWithAlpha(const Image* image, byte alpha);
    static void SetTexture(Image* texture);
};