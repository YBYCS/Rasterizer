#pragma once


#include "Point.h"
#include "General.h"

class Rasterizer
{
private:
    
public:
    Rasterizer();
    ~Rasterizer();
    static void DrawLine(Point p1, Point p2);
    
    static float TriangleArea(const Point &p1, const Point &p2, const Point &p3);

    static void DrawTriangle(Point p1, Point p2, Point p3);
    static void DrawTriangleEdge(Point p1, Point p2, Point p3);
    //普通的正交投影
    static Vec2 project(const Vec3 &vertex) {
        
        float screenX = (vertex.x + 1.0f) * 0.5f * 800; 
        float screenY = (vertex.y + 1.0f) * 0.5f * 600;
        return Vec2{screenX, screenY};
    }
};

