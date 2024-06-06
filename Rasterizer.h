#pragma once

#include "main.h"
#include "Point.h"
#include "Image.h"

class Rasterizer
{
private:
    static bool enableBlending; // 默认禁用混合
public:
    Rasterizer();
    ~Rasterizer();

    static void SetBlending(bool enabled);
    static bool IsBlendingEnabled();
    static void DrawLine(const Point& p1, const Point& p2);
    static float GetTriangleArea(const Point &p1, const Point &p2, const Point &p3);
    static void DrawTriangle(const Point& p1, const Point& p2, const Point& p3);
    static void DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3);
    static void DrawImage(const Image* image);
    static void DrawImageWithAlpha(const Image* image, byte alpha);
};