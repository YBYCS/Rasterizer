//
// Created by azuslai on 20/5/2024.
//

#ifndef RASTERIZER_COLOR_H
#define RASTERIZER_COLOR_H

#include <windows.h>

class Color {
public:
    int r, g, b,a;

    Color(int r = 255, int g = 255, int b = 255,int a=255) : r(r), g(g), b(b) ,a(a){}

    // 线性插值
    static Color Lerp(const Color &start, const Color &end, float t);

    unsigned long ToRGB() const ;
};


#endif //RASTERIZER_COLOR_H
