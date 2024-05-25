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

void Rasterizer::DrawLine(const Point& p1, const Point& p2) {
    int dx = abs(p2.x - p1.x);   //计算x方向的增量（绝对值）
    int dy = abs(p2.y - p1.y);   //计算y方向的增量（绝对值）
    int sx = (p1.x < p2.x) ? 1 : -1; //确定x方向的步长（正向或反向）
    int sy = (p1.y < p2.y) ? 1 : -1; //确定y方向的步长（正向或反向）
    
    //颜色插值权重
    float weight = 1.0f;

    // 检查 dx 和 dy 是否为零
    if (dx == 0) {
        // 垂直线
        for (int y = std::min(p1.y, p2.y); y <= std::max(p1.y, p2.y); ++y) {
            Point currentPoint(p1.x, y);
            weight = (float)(currentPoint.y - p1.y) / (float)(p2.y - p1.y);
            window->SetColorsbuff(currentPoint.x, currentPoint.y, Color::Lerp(p1.color, p2.color, weight).ToRGB());
        }
    } else if (dy == 0) {
        // 水平线
        for (int x = std::min(p1.x, p2.x); x <= std::max(p1.x, p2.x); ++x) {
            Point currentPoint(x, p1.y);
            weight = (float)(currentPoint.x - p1.x) / (float)(p2.x - p1.x);
            window->SetColorsbuff(currentPoint.x, currentPoint.y, Color::Lerp(p1.color, p2.color, weight).ToRGB());
        }
    } else {
        int err = dx - dy;

        Point currentPoint = p1;
        while (true) {
            window->SetColorsbuff(currentPoint.x, currentPoint.y, currentPoint.color.ToRGB());
            if (currentPoint.x == p2.x && currentPoint.y == p2.y) break;

            int err_ = 2 * err;
            if (err_ >= -dy) { err -= dy; currentPoint.x += sx; }
            if (err_ <= dx) { err += dx; currentPoint.y += sy; }

            weight = (float)(currentPoint.x - p1.x) / (float)(p2.x - p1.x);
            currentPoint.color = Color::Lerp(p1.color, p2.color, weight);
        }
    }
}

void Rasterizer::DrawTriangleEdge(const Point& p1, const Point& p2, const Point& p3) {
    // 画三角形的三条边
    DrawLine(p1, p2);
    DrawLine(p2, p3);
    DrawLine(p3, p1);
}
void Rasterizer::DrawTriangle(Point p1, Point p2, Point p3) {
    //根据y值来排布
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p1.y > p3.y) std::swap(p1, p3);
    if (p2.y > p3.y) std::swap(p2, p3);
    //计算x的边缘
    auto interpolate = [](int y, const Point& p1, const Point& p2)->float {
        if (p1.y == p2.y) return p1.x;
        return p1.x + (p2.x - p1.x) * (float(y - p1.y) / (p2.y - p1.y));
    };
    //重心插值
    auto colorInterpolate = [](int x, int y, const Point& p1, const Point& p2, const Point& p3) {
        float totalArea = TriangleArea(p1, p2, p3);
        float area1 = TriangleArea(Point(x, y), p2, p3);
        float area2 = TriangleArea(p1, Point(x, y), p3);
        float area3 = TriangleArea(p1, p2, Point(x, y));
        float alpha = area1 / totalArea;
        float beta = area2 / totalArea;
        float gamma = area3 / totalArea;

        byte r = alpha * p1.color.r + beta * p2.color.r + gamma * p3.color.r;
        byte g = alpha * p1.color.g + beta * p2.color.g + gamma * p3.color.g;
        byte b = alpha * p1.color.b + beta * p2.color.b + gamma * p3.color.b;
        
        return Color(r, g, b);
    };

    for (int y = p1.y; y <= p3.y; ++y) {
        if (y < 0 || y >= window->GetHeight()) continue;

        int x1 = (y < p2.y) ? interpolate(y, p1, p2) : interpolate(y, p2, p3);
        int x2 = interpolate(y, p1, p3);
        if (x1 > x2) std::swap(x1, x2);

        for (int x = x1; x <= x2; ++x) {
            if (x < 0 || x >= window->GetWidth()) continue;
            Color interpolatedColor = colorInterpolate(x, y, p1, p2, p3);
            window->SetColorsbuff(x, y, interpolatedColor.ToRGB());
        }
    }
}

float Rasterizer::TriangleArea(const Point& p1, const Point& p2, const Point& p3) {
    return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0f);
}