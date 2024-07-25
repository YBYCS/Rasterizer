#include "Rasterizer.h"
#include "main.h"
#include "Point.h"
#include <math.h>
#include "WindowController.h"
#include <iostream>
#include "Render.h"

Rasterizer::Rasterizer()
{
}

Rasterizer::~Rasterizer()
{
}

SamplingMethod Rasterizer::samplingMethod_ = SamplingMethod::NEAREST_NEIGHBOR;
TextureWarpMode Rasterizer::textureWarpMode_ = TextureWarpMode::REPEAT;

void Rasterizer::SetSamplingMethod(SamplingMethod method)
{
    samplingMethod_ = method;
}

SamplingMethod Rasterizer::GetSamplingMethod()
{
    return samplingMethod_;
}

void Rasterizer::SetTextureWarpMode(TextureWarpMode mode)
{
    textureWarpMode_ = mode;
}

TextureWarpMode Rasterizer::GetTextureWarpMode()
{
    return textureWarpMode_;
}

Color Rasterizer::Sampling(Vector2 texCoord, const Image *texture)
{
    Color res;
    switch (samplingMethod_)
    {
    case SamplingMethod::NEAREST_NEIGHBOR:
        res = SampleTextureNearest(texCoord, texture);
        break;
    case SamplingMethod::BILINEAR_INTERPOLATION:
        res = SampleTextureBilinear(texCoord, texture);
        break;
    default:
        res = SampleTextureNearest(texCoord, texture);
        break;
    }
    return res;
}

void Rasterizer::DrawImage(const Image *image)
{
    for (int i = 0; i < image->width; i++) {
        for (int j = 0; j < image->height; j++) {
            window->DrawPoint(i, j, image->colors[j * image->width + i]);
        }
    }
}

void Rasterizer::DrawImageWithAlpha(const Image *image, byte alpha)
{
    Color color;
    for (int i = 0; i < image->width; i++) {
        for (int j = 0; j < image->height; j++) {
            color = image->colors[j * image->width + i];
            color.a = alpha;
            window->DrawPoint(i, j, color);
        }
    }
}

float Rasterizer::GetTriangleArea(const Point& p1, const Point& p2, const Point& p3) 
{
    return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0f);
}

float Rasterizer::GetTriangleArea(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3)
{
    return abs((v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y)) / 2.0f);
}

//邻近过滤采样
Color Rasterizer::SampleTextureNearest(const Vector2 &uv, const Image *texture)
{
    auto tempUV = uv;
    WrapUV(tempUV);
    int x = std::round(tempUV.x * (texture->width - 1));
    int y = std::round(tempUV.y * (texture->height - 1));
    return texture->colors[y * texture->width + x];
}

//双线性插值采样
Color Rasterizer::SampleTextureBilinear(const Vector2 &uv, const Image *texture)
{
    auto tempUV = uv;
    WrapUV(tempUV);
    float x = tempUV.x * (texture->width - 1);
    float y = tempUV.y * (texture->height - 1);

    int left = std::floor(x);
    int right = std::ceil(x);
    int top = std::ceil(y);
    int bottom = std::floor(y);

    float ratioY = top == bottom ? 1.0f : (top - y) / (float)(top - bottom);
    
    Color leftColor = Color::Lerp(texture->colors[top * texture->width + left], texture->colors[bottom * texture->width + left], ratioY);
    Color rightColor = Color::Lerp(texture->colors[top * texture->width + right], texture->colors[bottom * texture->width + right], ratioY);
    
    float ratioX = left == right ? 1.0f : (x - left) / (float)(right - left);

    return Color::Lerp(leftColor, rightColor, ratioX);
}

void Wrap(float &x) 
{
    if (Rasterizer::GetTextureWarpMode() == TextureWarpMode::CLAMP) {
        if (x > 1.0f)
            x = 1.0f;
        else if (x < 0.0f)
            x = 0.0f;
        return;
    }

    if (x > 1.0f || x < 0.0f) {
        x = x - (int)x;
        switch(Rasterizer::GetTextureWarpMode()) 
        {
        case TextureWarpMode::REPEAT:
            x = (1 + x) - (int)(1 + x);
            break;
        case TextureWarpMode::MIRRORED_REPEAT:
            x = 1.0f - ((1 + x) - (int)(1 + x));
            break;
        default:
            break;
        }
    }
}
void Rasterizer::WrapUV(Vector2 &uv)
{
    Wrap(uv.x);
    Wrap(uv.y);
}

void Rasterizer::RasterizeLine(std::vector<VertexData> &output, const VertexData &v1, const VertexData &v2)
{
    int x1 = static_cast<int>(v1.position.x);
    int y1 = static_cast<int>(v1.position.y);
    int x2 = static_cast<int>(v2.position.x);
    int y2 = static_cast<int>(v2.position.y);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);

    //两点重合
    if (dx == 0 && dy == 0) {
        VertexData data;
        data.position = v1.position;
        data.color = v1.color * 0.5f + v2.color * 0.5f;
        data.normal = v1.normal * 0.5f + v2.normal * 0.5f;
        data.texCoord = v1.texCoord * 0.5f + v2.texCoord * 0.5f;
        output.emplace_back(data);
        return;
    }

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    while (true) {
        float t = (dx > dy) ? static_cast<float>(x - x1) / dx : static_cast<float>(y - y1) / dy;

        VertexData currentVertex;
        currentVertex.position = { (float)x, (float)y, 0.0f, 1.0f };
        currentVertex.color = v1.color * (1 - t) + v2.color * t;
        currentVertex.normal = v1.normal * (1 - t) + v2.normal * t;
        currentVertex.texCoord = v1.texCoord * (1 - t) + v2.texCoord * t;
        output.push_back(currentVertex);

        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

void Rasterizer::RasterizeTriangle(std::vector<VertexData> &output, const VertexData &v0, const VertexData &v1, const VertexData &v2, const Image* texture)
{
    const float sampleOffsets[4][2] = {{0.25f, 0.25f}, {0.25f, 0.75f}, {0.75f, 0.25f}, {0.75f, 0.75f}};
    //改用了更加先进的 Edge Equation 算法
    int maxX = static_cast<int>(std::max(v0.position.x, std::max(v1.position.x, v2.position.x)));
    int minX = static_cast<int>(std::min(v0.position.x, std::min(v1.position.x, v2.position.x)));
    int maxY = static_cast<int>(std::max(v0.position.y, std::max(v1.position.y, v2.position.y)));
    int minY = static_cast<int>(std::min(v0.position.y, std::min(v1.position.y, v2.position.y)));

    //改用了更加精确的求三角形面积公式
    auto e1 = Vector2(v1.position.x - v0.position.x, v1.position.y - v0.position.y);
    auto e2 = Vector2(v2.position.x - v0.position.x, v2.position.y - v0.position.y);
    float totalArea = std::abs(Cross(e1, e2));

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            int num = 0;
            Vector2 pv0 = Vector2(v0.position.x - (x + 0.5f), v0.position.y - (y + 0.5f));
            Vector2 pv1 = Vector2(v1.position.x - (x + 0.5f), v1.position.y - (y + 0.5f));
            Vector2 pv2 = Vector2(v2.position.x - (x + 0.5f), v2.position.y - (y + 0.5f));

            auto cross1 = Cross(pv0, pv1);
            auto cross2 = Cross(pv1, pv2);
            auto cross3 = Cross(pv2, pv0);

            bool negativeAll = cross1 <= 0 && cross2 <= 0 && cross3 <= 0;
            bool positiveAll = cross1 >= 0 && cross2 >= 0 && cross3 >= 0;

            bool flag1 = negativeAll || positiveAll;

            for (int sample = 0; sample < 4; ++sample) {
                float sampleX = x + sampleOffsets[sample][0];
                float sampleY = y + sampleOffsets[sample][1];
                
                Vector2 pv0 = Vector2(v0.position.x - sampleX, v0.position.y - sampleY);
                Vector2 pv1 = Vector2(v1.position.x - sampleX, v1.position.y - sampleY);
                Vector2 pv2 = Vector2(v2.position.x - sampleX, v2.position.y - sampleY);
                
                auto cross1 = Cross(pv0, pv1);
                auto cross2 = Cross(pv1, pv2);
                auto cross3 = Cross(pv2, pv0);

                bool negativeAll = cross1 <= 0 && cross2 <= 0 && cross3 <= 0;
                bool positiveAll = cross1 >= 0 && cross2 >= 0 && cross3 >= 0;

                if (negativeAll || positiveAll) {
                    //该采样点在三角形内，计算深度
                    float alpha = std::abs(Cross(pv1, pv2)) / totalArea;
                    float beta = std::abs(Cross(pv0, pv2)) / totalArea;
                    float gamma = std::abs(Cross(pv0, pv1)) / totalArea;

                    float z = alpha * v0.position.z + beta * v1.position.z + gamma * v2.position.z;
                    if (z < Render::GetDepthFromMsaa(x, y, sample)) {
                        num++;
                        Render::SetMsaaDepth(x, y, sample, z);
                        //设置颜色
                        float oneOverW = alpha * v0.oneOverW + beta * v1.oneOverW + gamma * v2.oneOverW;
                        Vector2 texCoord = (alpha * v0.texCoord + beta * v1.texCoord + gamma * v2.texCoord) /= oneOverW;
                        Vector4 color = texture ? ColorToVector4(Sampling(texCoord, texture)) : (alpha * v0.color + beta * v1.color + gamma * v2.color);   
                        Render::SetMsaaColor(x, y, sample, color);
                    }
                }
            }
            if (num > 0 || flag1) {
                VertexData currentVertex;
                currentVertex.position.x = x;
                currentVertex.position.y = y;

                float alpha = std::abs(Cross(pv1, pv2)) / totalArea;
                float beta = std::abs(Cross(pv0, pv2)) / totalArea;
                float gamma = std::abs(Cross(pv0, pv1)) / totalArea;

                currentVertex.oneOverW = alpha * v0.oneOverW + beta * v1.oneOverW + gamma * v2.oneOverW;
                currentVertex.position.z = alpha * v0.position.z + beta * v1.position.z + gamma * v2.position.z;
                currentVertex.texCoord = (alpha * v0.texCoord + beta * v1.texCoord + gamma * v2.texCoord) /= currentVertex.oneOverW;
                currentVertex.normal = alpha * v0.normal + beta * v1.normal + gamma * v2.normal;
                //颜色在下一轮额外处理
                output.emplace_back(currentVertex);
            }
        }
    }
}

void Rasterizer::RasterizeTriangle2(std::vector<VertexData> &output)
{
    for(auto &vertex : output) {
        int x = vertex.position.x;
        int y = vertex.position.y;
        vertex.color = ((Render::GetColorFromMsaa(x, y, 0) + Render::GetColorFromMsaa(x, y, 1) + Render::GetColorFromMsaa(x, y, 2) + Render::GetColorFromMsaa(x, y, 3)) / 4);
    }
}

std::vector<VertexData> Rasterizer::Rasterize(DrawMode drawMode, const std::vector<VertexData>& input, const Image* texture)
{
    std::vector<VertexData> output;
    switch (drawMode)
    {
    case DrawMode::LINE:
        for (size_t i = 0; i < input.size(); i += 2) {
            RasterizeLine(output, input[i], input[i + 1]);
        }
        break;
    case DrawMode::TRIANGLE:
        for (size_t i = 0; i < input.size(); i += 3) {
            RasterizeTriangle(output, input[i], input[i + 1], input[i + 2], texture);
        }
        RasterizeTriangle2(output);
        break;
    default:
        break;
    }
    return output;
}