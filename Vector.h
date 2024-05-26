#include <math.h>

class Vector3D;
class Vector4D;

class Vector2D {
public:
    float x, y;

    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}
    Vector2D(const Vector3D& v) : x(v.x), y(v.y) {}
    Vector2D(const Vector4D& v) : x(v.x), y(v.y) {}

    Vector2D operator=(const Vector2D& v) { x = v.x; y = v.y; return *this; }
    Vector2D operator=(const Vector3D& v) { x = v.x; y = v.y; return *this; }
    Vector2D operator=(const Vector4D& v) { x = v.x; y = v.y; return *this; }

    Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
    Vector2D operator-(const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
    Vector2D operator*(float scalar) const { return Vector2D(x * scalar, y * scalar); }   //标量乘法
    Vector2D operator/(float scalar) const { return Vector2D(x / scalar, y / scalar); }   //标量除法

    Vector2D operator+=(const Vector2D& v) { x += v.x; y += v.y; return *this; }
    Vector2D operator-=(const Vector2D& v) { x -= v.x; y -= v.y; return *this; }
    Vector2D operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    Vector2D operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    Vector2D operator-() { return Vector2D(-x, -y); }
    //点积
    float Dot(const Vector2D& v) const { return x * v.x + y * v.y; } 
    //叉积
    Vector2D Cross(const Vector2D& v) const { return x * v.y - y * v.x; }
    //模长
    float Length() const { return sqrt(x * x + y * y); }
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
    Vector3D(const Vector4D& v) : x(v.x), y(v.y), z(z) {}

    Vector3D operator=(const Vector2D& v) { x = v.x; y = v.y; z = 0; return *this; }
    Vector3D operator=(const Vector3D& v) { x = v.x; y = v.y; z = v.z; return *this; }
    Vector3D operator=(const Vector4D& v) { x = v.x; y = v.y; z = v.z; return *this; }

    Vector3D operator+(const Vector3D& v) const { return Vector3D(x + v.x, y + v.y, z + v.z); }
    Vector3D operator-(const Vector3D& v) const { return Vector3D(x - v.x, y - v.y, z - v.z); }
    Vector3D operator*(float scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }
    Vector3D operator/(float scalar) const { return Vector3D(x / scalar, y / scalar, z / scalar); }

    Vector3D operator+=(const Vector3D& v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3D operator-=(const Vector3D& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3D operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    Vector3D operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

    Vector3D operator-() { return Vector3D(-x, -y, -z); }
    //点积
    float Dot(const Vector3D& v) const {  return x * v.x + y * v.y + z * v.z; } 
    //叉积
    Vector3D Cross(const Vector3D& v) const {
        return Vector3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    //模长
    float Length() const { return sqrt(x * x + y * y + z * z); }
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
    //模长
    float Length() const { return sqrt(x * x + y * y + z * z + w * w); }
    //标准化
    Vector4D Normalize() {
        float len = Length();
        if (len != 0) {
            x /= len; y /= len; z /= len; w /= len;
        }
        return *this;
    }
};