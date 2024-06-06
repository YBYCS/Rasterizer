//
// Created by azuslai on 20/5/2024.
//

#include "Color.h"
#include <wingdi.h>
#include <windows.h>

Color Color::Lerp(const Color &start, const Color &end, float t)
{
    return Color(
            static_cast<byte>(static_cast<float>(end.r) * t + (1 - t) * static_cast<float>(start.r)),
            static_cast<byte>(static_cast<float>(end.g) * t + (1 - t) * static_cast<float>(start.g)),
            static_cast<byte>(static_cast<float>(end.b) * t + (1 - t) * static_cast<float>(start.b)),
            static_cast<byte>(static_cast<float>(end.a) * t + (1 - t) * static_cast<float>(start.a))
    );
}