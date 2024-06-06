#include "Rasterizer.h"
#include "main.h"
#include "Point.h"
#include <math.h>
#include "WindowController.h"

Rasterizer::Rasterizer()
{
}

Rasterizer::~Rasterizer()
{
}

bool Rasterizer::enableBlending = false;

void Rasterizer::SetBlending(bool enabled)
{
    enableBlending = enabled;
}

bool Rasterizer::IsBlendingEnabled()
{
    return enableBlending;
}

void Rasterizer::DrawLine(const Point& p1, const Point& p2) {
    int dx = abs(p2.x - p1.x);   //计算x方向的增量（绝对值）
    int dy = abs(p2.y - p1.y);   //计算y方向的增量（绝对值）

    //两点重合
    if (dx == 0 && dy == 0) {
        window->DrawPoint(p1.x, p1.y, Color::Lerp(p1.color, p2.color, 0.5f));
        return;
    }

    int sx = (p1.x < p2.x) ? 1 : -1; //确定x方向的步长（正向或反向）
    int sy = (p1.y < p2.y) ? 1 : -1; //确定y方向的步长（正向或反向）

    int err = dx - dy;

    Point currentPoint = p1;
    while (true) {
        window->DrawPoint(currentPoint.x, currentPoint.y, currentPoint.color);
        if (currentPoint.x == p2.x && currentPoint.y == p2.y) break;

        int err_ = 2 * err;
        if (err_ >= -dy) { err -= dy; currentPoint.x += sx; }   //朝x方向移动一个单位，减少了y方向上的偏差
        if (err_ <= dx) { err += dx; currentPoint.y += sy; }    //朝y方向移动一个单位，减少了x方向上的偏差

        float weight = (dx > dy) 
            ? (float)(currentPoint.x - p1.x) / (float)(p2.x - p1.x)
            : (float)(currentPoint.y - p1.y) / (float)(p2.y - p1.y);
        currentPoint.color = Color::Lerp(p1.color, p2.color, weight);
    }
}

void Rasterizer::DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3) {
    // 画三角形的三条边
    DrawLine(p1, p2);
    DrawLine(p2, p3);
    DrawLine(p3, p1);
}

void Rasterizer::DrawImage(const Image *image)
{
    for (int i = 0; i < image->width; i++) {
        for (int j = 0; j < image->height; j++) {
            window->DrawPoint(i, j, image->color[j * image->width + i]);
        }
    }
}

void Rasterizer::DrawImageWithAlpha(const Image *image, byte alpha)
{
    Color color;
    for (int i = 0; i < image->width; i++) {
        for (int j = 0; j < image->height; j++) {
            color = image->color[j * image->width + i];
            color.a = alpha;
            window->DrawPoint(i, j, color);
        }
    }
}

void Rasterizer::DrawTriangle(const Point& p1, const Point& p2, const Point& p3) {
    float totalArea = GetTriangleArea(p1, p2, p3);
    //重心插值
    auto colorInterpolate = [totalArea](int x, int y, const Point& p1, const Point& p2, const Point& p3) {
        float area1 = GetTriangleArea(Point(x, y), p2, p3);
        float area2 = GetTriangleArea(p1, Point(x, y), p3);
        float area3 = GetTriangleArea(p1, p2, Point(x, y));
        float alpha = area1 / totalArea;
        float beta = area2 / totalArea;
        float gamma = area3 / totalArea;

        byte r = alpha * static_cast<float>(p1.color.r) + beta * static_cast<float>(p2.color.r) + gamma * static_cast<float>(p3.color.r);
        byte g = alpha * static_cast<float>(p1.color.g) + beta * static_cast<float>(p2.color.g) + gamma * static_cast<float>(p3.color.g);
        byte b = alpha * static_cast<float>(p1.color.b) + beta * static_cast<float>(p2.color.b) + gamma * static_cast<float>(p3.color.b);
        byte a = alpha * static_cast<float>(p1.color.a) + beta * static_cast<float>(p2.color.a) + gamma * static_cast<float>(p3.color.a);

        return Color(r, g, b, a);
    };

    Point p[3] = { p1, p2, p3 };
    //根据y值来排布
    if (p[0].y > p[1].y) std::swap(p[0], p[1]);
    if (p[0].y > p[2].y) std::swap(p[0], p[2]);
    if (p[1].y > p[2].y) std::swap(p[1], p[2]);

    //计算x的边缘
    auto interpolate = [](int y, const Point& p1, const Point& p2)->float {
        if (p1.y == p2.y) return p1.x;
        return p1.x + (p2.x - p1.x) * (float(y - p1.y) / (p2.y - p1.y));
    };

    for (int y = p[0].y; y <= p[2].y; ++y) {
        if (y < 0 || y >= window->GetHeight()) continue;

        int x1 = (y < p[1].y) ? interpolate(y, p[0], p[1]) : interpolate(y, p[1], p[2]);
        int x2 = interpolate(y, p[0], p[2]);
        if (x1 > x2) std::swap(x1, x2);

        for (int x = x1; x <= x2; ++x) {
            if (x < 0 || x >= window->GetWidth()) continue;
            Color interpolatedColor = colorInterpolate(x, y, p[0], p[1], p[2]);
            window->DrawPoint(x, y, interpolatedColor);
        }
    }
}

float Rasterizer::GetTriangleArea(const Point& p1, const Point& p2, const Point& p3) {
    return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0f);
}