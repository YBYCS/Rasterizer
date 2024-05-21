//
// Created by azuslai on 20/5/2024.
//

#ifndef RASTERIZER_POINT_H
#define RASTERIZER_POINT_H

#include "Color.h"

class Point {
public:
    int x, y;
    Color color;

    Point(int x, int y, Color color) : x(x), y(y), color(color) {}

    Point(int x, int y, int red, int green, int blue, int alpha)
            : x(x), y(y), color(Color(red, green, blue, alpha)) {}

    Point(int x, int y) : Point(x, y, Color()) {}
    static Point Interpolate(const Point& a, const Point& b, float t) {
        int newX = a.x + static_cast<int>(t * (b.x - a.x));
        int newY = a.y + static_cast<int>(t * (b.y - a.y));
        Color newColor = Color::Lerp(a.color, b.color, t);
        return Point(newX, newY, newColor);
    }
};


#endif //RASTERIZER_POINT_H
