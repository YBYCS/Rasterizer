#pragma once
#include "Color.h"
#include <string>
#include <memory>

class Image {
public:
    int width = 0;
    int height = 0;
    std::unique_ptr<Color[]> colors;
public:
    Image(int width = 0, int height = 0, Color* data = nullptr);
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    ~Image();

    static std::unique_ptr<Image> CreateImage(const std::string& path);
};