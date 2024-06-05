#pragma once
#include "Color.h"
#include <string>

class Image {
public:
    int width = 0;
    int height = 0;
    Color* color = nullptr;
public:
    Image(int width = 0, int height = 0, Color* data = nullptr);
    ~Image();

    static Image* CreateImage(const std::string& path);
    static void DestoryImage(Image* image);
};