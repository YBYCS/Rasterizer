#include "Vector.h"
#include "WindowController.h"
class Matrix4;

// OpenGL中矩阵是列优先存储的。即：
// m0, m3, m6
// m1, m4, m7
// m2, m5, m8

class Matrix3 {
public:
    float m[9] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
public:
    Matrix3(float v = 1.0f);
    Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
    Matrix3(const Matrix3& other);
    Matrix3(const Matrix4& other);
    Matrix3 operator*(float s) const;
    Vector3 operator*(const Vector3& v) const;
    Matrix3 operator*(const Matrix3& other) const;
    void Set(int row, int col, float t);
    void Set(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);
    Matrix3 Identity();
    Vector3 GetColum(int col) const;
    void SetColum(int col, const Vector3& v);
};

class Matrix4 {
public:
    float m[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
public:
    Matrix4(float v = 1.0f);
    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);
    Matrix4(const Matrix4& other);
    Matrix4 operator*(float s) const;
    Vector4 operator*(const Vector4& v) const;
    Matrix4 operator*(const Matrix4& other) const;
    float Get(int row, int col) const;
    void Set(int row, int col, float t);
    void Set(float m00, float m01, float m02, float m03,
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33);
    Matrix3 Identity();
    Vector4 GetColum(int col) const;
    void SetColum(int col, const Vector4& v);
};

//返回参数的转置矩阵
Matrix3 Transpose(const Matrix3& matrix);
//返回参数的转置矩阵
Matrix4 Transpose(const Matrix4& matrix);
//返回参数的逆矩阵
Matrix4 Inverse(const Matrix4& matrix);
void PrintMatrix(const Matrix3& matrix);
void PrintMatrix(const Matrix4& matrix);
//对矩阵进行缩放
Matrix4 Scale(const Matrix4& matrix, float x, float y, float z);
//对矩阵进行平移，参数为模型空间下的x、y、z距离
Matrix4 Translate(const Matrix4& matrix, float x, float y, float z);
Matrix4 Translate(const Matrix4& matrix, const Vector3& v);
//绕着任意轴旋转矩阵
Matrix4 RotateMartix(const Matrix4& matrix, const Vector3& v, float angle);
//根据投影盒的参数生成正交投影矩阵
Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);
//根据视锥体的参数生成透视投影矩阵
Matrix4 Perspective(float fovy, float n, float f, float aspect = (float)window->GetWidth() / (float)window->GetHeight());
//屏幕空间变换矩阵
Matrix4 ScreenMatrix(int width = window->GetWidth(), int height = window->GetHeight());