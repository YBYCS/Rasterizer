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

void Rasterizer::DrawLine(Point p1, Point p2) {
    int dx = abs(p2.x - p1.x), sx = p1.x < p2.x ? 1 : -1;
    int dy = -abs(p2.y - p1.y), sy = p1.y < p2.y ? 1 : -1;
    float steps = std::max(dx, abs(dy));
    for (float t = 0; t <= 1; t += 1.0f / steps) {
        Point currentPoint = Point::Interpolate(p1, p2, t);
        window->SetColorsbuff(currentPoint.x, currentPoint.y, currentPoint.color.ToRGB());
    }
}
void Rasterizer::DrawTriangleEdge(Point p1, Point p2, Point p3) {
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

        int r = alpha * p1.color.r + beta * p2.color.r + gamma * p3.color.r;
        int g = alpha * p1.color.g + beta * p2.color.g + gamma * p3.color.g;
        int b = alpha * p1.color.b + beta * p2.color.b + gamma * p3.color.b;
        
        return Color(r, g, b);
    };

    for (int y = p1.y; y <= p3.y; ++y) {
        if (y < 0 || y >= WINDOW_HEIGHT) continue;

        int x1 = (y < p2.y) ? interpolate(y, p1, p2) : interpolate(y, p2, p3);
        int x2 = interpolate(y, p1, p3);
        if (x1 > x2) std::swap(x1, x2);

        for (int x = x1; x <= x2; ++x) {
            if (x < 0 || x >= WINDOW_WIDTH) continue;
            Color interpolatedColor = colorInterpolate(x, y, p1, p2, p3);
            window->SetColorsbuff(x, y, interpolatedColor.ToRGB());
        }
    }
}

float Rasterizer::TriangleArea(const Point& p1, const Point& p2, const Point& p3) {
    return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2.0f);
}