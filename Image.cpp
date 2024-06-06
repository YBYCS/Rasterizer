#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include <iostream>

Image::Image(int width, int height, Color *color) : width(width), height(height), colors(nullptr)
{
    if (color) {
        colors = std::make_unique<Color[]>(width * height);
        memcpy(colors.get(), color, sizeof(Color) * width * height);
    }
}

Image::~Image() {}

std::unique_ptr<Image> Image::CreateImage(const std::string &path)
{
    int width = 0, height = 0, type = 0;

    //stbimage读入图片默认原点在左上角，因此需要上下反转
    stbi_set_flip_vertically_on_load(true);
    //参数为： 1.图片所处路径、2.图片长度、3.图片高度、4.图片类型、5.以RGBA格式读取每一个像素的RGBA
    auto* bits = stbi_load(path.c_str(), &width, &height, &type, STBI_rgb_alpha);  

    if (bits == nullptr) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }   

    //图片默认颜色通道为BGRA，GDI默认颜色通道为RGBA，因此需要将图片中每个像素的 R 和 B交换
    for (int i = 0; i < width * height * 4; i += 4) {
        std::swap(bits[i], bits[i + 2]);
    }

    auto image = std::make_unique<Image>(width, height, (Color*)bits);
    //上一行代码已经将bits拷贝到image里了，这一行代码用于删掉bits
    stbi_image_free(bits);  
    return image;
}