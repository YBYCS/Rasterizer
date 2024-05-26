#include <math.h>

class Vector3D;
class Vector4D;

class Vector2D {
public:
    float x, y;

    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}

    Vector2D operator=(const Vector2D& v) { x = v.x; y = v.y; return *this; }
    Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
    Vector2D operator-(const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }   //标量乘法
    Vector2D operator/(float scalar) const { return Vector2D(x / scalar, y / scalar); }   //标量除法

    Vector2D operator+=(const Vector2D& v) { x += v.x; y += v.y; return *this; }
    Vector2D operator-=(const Vector2D& v) { x -= v.x; y -= v.y; return *this; }
    Vector2D operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2D operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    Vector2D operator-() { return Vector2D(-x, -y); }
    Vector2D operator*(const Vector2D& v) const { return Vector2D(x * v.x, y *v.y); }
    //模长的平方
    float lengthSquared() const { return x * x + y * y; }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector2D Normalize() {
        float len = Length();
        if (len != 0)
            x /= len; y /= len;
        return *this;
    }
};

class Vector3D {
public:
    float x, y, z;

    Vector3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vector3D(const Vector2D& v) : x(v.x), y(v.y), z(0) {}
    Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(z) {}

    Vector3D operator=(const Vector2D& v) { x = v.x; y = v.y; z = 0; return *this; }
    Vector3D operator=(const Vector3D& v) { x = v.x; y = v.y; z = v.z; return *this; }

    Vector3D operator+(const Vector3D& v) const { return Vector3D(x + v.x, y + v.y, z + v.z); }
    Vector3D operator-(const Vector3D& v) const { return Vector3D(x - v.x, y - v.y, z - v.z); }
    Vector3D operator*(float scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
    Vector3D operator/(float scalar) const { return Vector3D(x / scalar, y / scalar, z / scalar); }

    Vector3D operator+=(const Vector3D& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3D operator-=(const Vector3D& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3D operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3D operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    Vector3D operator-() { return Vector3D(-x, -y, -z); }
    Vector3D operator*(const Vector3D& v) const { return Vector3D(x * v.x, y * v.y, z * v.z); }
    //模长的平方
    float lengthSquared() const { return sqrt(x * x + y * y + z * z); }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector3D Normalize() {
        float len = Length();
        if (len != 0)
            x /= len; y /= len; z /= len;
        return *this;
    }
};

class Vector4D {
public:
    float x, y, z, w;

    Vector4D(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
    Vector4D(const Vector2D& v) : x(v.x), y(v.y), z(0), w(0) {}
    Vector4D(const Vector3D& v) : x(v.x), y(v.y), z(v.z), w(0) {}
    Vector4D(const Vector4D& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    Vector4D operator=(const Vector2D& v) { x = v.x; y = v.y; z = 0; w = 0; return *this; }
    Vector4D operator=(const Vector3D& v) { x = v.x; y = v.y; z = v.z; w = 0; return *this; }
    Vector4D operator=(const Vector4D& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

    Vector4D operator+(const Vector4D& v) const { return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w); }
    Vector4D operator-(const Vector4D& v) const { return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w); }
    Vector4D operator*(float scalar) const { return Vector4D(x * scalar, y * scalar, z * scalar, w * scalar); }
    Vector4D operator/(float scalar) const { return Vector4D(x / scalar, y / scalar, z / scalar, w / scalar); } 

    Vector4D operator+=(const Vector4D& v) { x += v.x; y += v.y; z += v.z; w += v.z; return *this; }
    Vector4D operator-=(const Vector4D& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.z; return *this; }
    Vector4D operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
    Vector4D operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

    Vector4D operator-() { return Vector4D(-x, -y, -z, -w); }
    Vector4D operator*(const Vector4D& v) { return Vector4D(x * v.x, y * v.y, z * v.z, w * v.w); }
    //模长的平方
    float lengthSquared() const { return x * x + y * y + z * z + w * w; }
    //模长
    float Length() const { return sqrt(lengthSquared()); }
    //标准化
    Vector4D Normalize() {
        float len = Length();
        if (len != 0) {
            x /= len; y /= len; z /= len; w /= len;
        }
        return *this;
    }
};

inline Vector2D abs(const Vector2D& v) { return Vector2D(std::abs(v.x), std::abs(v.y)); }
inline Vector3D abs(const Vector3D& v) { return Vector3D(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }
inline Vector4D abs(const Vector4D& v) { return Vector4D(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w)); }

inline float Dot(const Vector2D& v1, const Vector2D& v2) { return v1.x * v2.x + v1.y * v2.y; };
inline float Dot(const Vector3D& v1, const Vector3D& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
inline float Dot(const Vector4D& v1, const Vector4D& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline float Cross(const Vector2D& v1, const Vector2D& v2) { return v1.x * v2.y - v1.y * v2.x; }
inline Vector3D Cross(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}
//二维向量的叉积是一个标量，三维向量的叉积是一个向量，四维向量没有叉积