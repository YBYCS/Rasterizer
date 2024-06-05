#pragma once

#include "main.h"
#include "Point.h"
#include "Image.h"

class Rasterizer
{
private:
    
public:
    Rasterizer();
    ~Rasterizer();
    static void DrawLine(const Point& p1, const Point& p2);
    
    static float TriangleArea(const Point &p1, const Point &p2, const Point &p3);

    static void DrawTriangle(const Point& p1, const Point& p2, const Point& p3);
    static void DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3);
    static void DrawImage(const Image* image);
    //普通的正交投影
    static Vector2 project(const Vector3 &vertex) {
        float screenX = (vertex.x + 1.0f) * 0.5f * window->GetWidth(); 
        float screenY = (vertex.y + 1.0f) * 0.5f * window->GetHeight();
        return Vector2{ screenX, screenY };
    }
};