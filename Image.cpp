#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include <iostream>

Image::Image(int width, int height, Color *color)
{
    this->width = width;
    this->height = height;
    if (color) {
        this->color = new Color[width * height];
        memcpy(this->color, color, sizeof(Color) * width * height);
    }
}

Image::~Image()
{
    if (color != nullptr)
        delete[] color;
}

Image *Image::CreateImage(const std::string &path)
{
    int width = 0, height = 0, type = 0;

    //stbimage读入图片默认原点在左上角，因此需要上下反转
    stbi_set_flip_vertically_on_load(true);
    //参数为： 1.图片所处路径、2.图片长度、3.图片高度、4.图片类型、5.以RGBA格式读取
    auto* bits = stbi_load(path.c_str(), &width, &height, &type, STBI_rgb_alpha);  

    if (bits == nullptr) {
        std::cerr << "Error loading image: " << stbi_failure_reason() << std::endl;
        return nullptr;
    }

    Image* image = new Image(width, height, (Color*)bits);
    //上一行代码已经将bits拷贝到image里了，这一行代码用于删掉bits
    stbi_image_free(bits);  
    return image;
}

void Image::DestoryImage(Image *image)
{
    if (image) 
        delete image;
}
