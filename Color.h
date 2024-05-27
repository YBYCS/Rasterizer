//
// Created by azuslai on 20/5/2024.
//

#ifndef RASTERIZER_COLOR_H
#define RASTERIZER_COLOR_H

#include <windows.h>

class Color {
public:
    byte r, g, b, a;

    Color(byte r = 255, byte g = 255, byte b = 255, byte a=255) : r(r), g(g), b(b) ,a(a){}

    // 线性插值
    static Color Lerp(const Color &start, const Color &end, float t);

    unsigned long ToBGR() const ;
};


#endif //RASTERIZER_COLOR_H
