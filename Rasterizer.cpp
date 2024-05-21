#include "Rasterizer.h"
#include "main.h"
#include "Point.h"
#include <math.h>

Rasterizer::Rasterizer()
{
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::DrawLine(Point p1, Point p2) {
    int dx = abs(p2.x - p1.x), sx = p1.x < p2.x ? 1 : -1;
    int dy = -abs(p2.y - p1.y), sy = p1.y < p2.y ? 1 : -1;
    int err = dx + dy, e2;

    float steps = std::max(dx, abs(dy));
    for (float t = 0; t <= 1; t += 1.0f / steps) {
        Point currentPoint = Point::Interpolate(p1, p2, t);
        // 假设colorsbuff是一个二维数组，用于存储颜色信息
        colorsbuff[currentPoint.x][currentPoint.y] = currentPoint.color.ToRGB(); // 需要实现Color类的ToRGB()方法
    }
}