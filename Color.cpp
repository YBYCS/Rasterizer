//
// Created by azuslai on 20/5/2024.
//

#include "Color.h"
#include <wingdi.h>
#include <windows.h>

Color Color::Lerp(const Color &start, const Color &end, float t)
{
    return Color(
                start.r + static_cast<byte>(t * (end.r - start.r)),
                start.g + static_cast<byte>(t * (end.g - start.g)),
                start.b + static_cast<byte>(t * (end.b - start.b))
        );
}

unsigned long Color::ToRGB() const
{
    return RGB(b, g, r);    //GDI中默认颜色通道是BGRA
}
