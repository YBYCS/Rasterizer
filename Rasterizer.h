#pragma once


#include "Point.h"

class Rasterizer
{
private:
    
public:
    Rasterizer();
    ~Rasterizer();
    static void DrawLine(Point p1, Point p2);
    
    static float TriangleArea(const Point &p1, const Point &p2, const Point &p3);

    static void DrawTriangle(Point p1, Point p2, Point p3);
};

