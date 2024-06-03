//
// Created by azuslai on 20/5/2024.
//

#ifndef RASTERIZER_POINT_H
#define RASTERIZER_POINT_H

#include "Color.h"
#include "Vector.h"

class Point {
public:
    int x, y;
    Color color;
    Point(const Vector2& v) : Point(v.x, v.y) {}
    Point(int x, int y) : Point(x, y, Color()) {}
    Point(int x, int y, Color color) : x(x), y(y), color(color) {}
    Point(int x, int y, byte red, byte green, byte blue, byte alpha)
            : x(x), y(y), color(Color(red, green, blue, alpha)) {}
};


#endif //RASTERIZER_POINT_H
