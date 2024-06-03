#pragma once
#include <math.h>

class Vector3;
class Vector4;

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2(const Vector2& v) : x(v.x), y(v.y) {}

    Vector2 operator=(const Vector2& v) { x = v.x; y = v.y; return *this; }
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }   //标量乘法
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }   //标量除法

    Vector2 operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
    Vector2 operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
    Vector2 operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2 operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    Vector2 operator-() { return Vector2(-x, -y); }
    //模长的平方
    float lengthSquared() const { return x * x + y * y; }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector2 Normalize() { return *this / Length(); }
};

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3(const Vector2& v) : x(v.x), y(v.y), z(0) {}
    Vector3(const Vector3& v) : x(v.x), y(v.y), z(z) {}

    Vector3 operator=(const Vector2& v) { x = v.x; y = v.y; z = 0; return *this; }
    Vector3 operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

    Vector3 operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3 operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3 operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3 operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    Vector3 operator-() { return Vector3(-x, -y, -z); }
    //模长的平方
    float lengthSquared() const { return sqrt(x * x + y * y + z * z); }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector3 Normalize() { return *this / Length(); }
};

class Vector4 {
public:
    float x, y, z, w;

    Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector2& v) : x(v.x), y(v.y), z(0), w(0) {}
    Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1) {}
    Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    Vector4 operator=(const Vector2& v) { x = v.x; y = v.y; z = 0; w = 0; return *this; }
    Vector4 operator=(const Vector3& v) { x = v.x; y = v.y; z = v.z; w = 1; return *this; }
    Vector4 operator=(const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

    Vector4 operator+(const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vector4 operator-(const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vector4 operator*(float scalar) const { return Vector4(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vector4 operator/(float scalar) const { return Vector4(x / scalar, y / scalar, z / scalar, w / scalar); } 

    Vector4 operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.z; return *this; }
    Vector4 operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.z; return *this; }
    Vector4 operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
    Vector4 operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

    Vector4 operator-() { return Vector4(-x, -y, -z, -w); }
    //模长的平方
    float lengthSquared() const { return x * x + y * y + z * z + w * w; }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector4 Normalize() { return *this / Length(); }
};

inline Vector2 operator*(float s, const Vector2& v) { return v * s; }
inline Vector3 operator*(float s, const Vector3& v) { return v * s; }
inline Vector4 operator*(float s, const Vector4& v) { return v * s; }

inline Vector2 operator*(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x * v2.x, v1.y * v2.y); }
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
inline Vector4 operator*(const Vector4& v1, const Vector4& v2) { return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w); }

inline Vector2 Normalize(const Vector2& v) { return v / v.Length(); }
inline Vector3 Normalize(const Vector3& v) { return v / v.Length(); }
inline Vector4 Normalize(const Vector4& v) { return v / v.Length(); }

inline Vector2 abs(const Vector2& v) { return Vector2(std::abs(v.x), std::abs(v.y)); }
inline Vector3 abs(const Vector3& v) { return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }
inline Vector4 abs(const Vector4& v) { return Vector4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w)); }

//以下分别是点乘和叉乘，注意 * 是逐元素相乘，不属于点乘或叉乘

inline float Dot(const Vector2& v1, const Vector2& v2) { return v1.x * v2.x + v1.y * v2.y; };
inline float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline float Dot(const Vector4& v1, const Vector4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline float Cross(const Vector2& v1, const Vector2& v2) { return v1.x * v2.y - v1.y * v2.x; }
inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}
//二维向量的叉积是一个标量，三维向量的叉积是一个向量，四维向量没有叉积