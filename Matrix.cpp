#include "Matrix.h"
#include <algorithm>
#include <iterator>
#include <cassert>
#include <iostream>

Matrix3::Matrix3(float v) 
{
    std::fill(std::begin(m), std::end(m), 0.0f);
    m[0] = m[4] = m[8] = v;
}

Matrix3::Matrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    m[0] = m00; m[3] = m01; m[6] = m02;
    m[1] = m10; m[4] = m11; m[7] = m12;
    m[2] = m20; m[5] = m21; m[8] = m22;
}

Matrix3::Matrix3(const Matrix3& other) 
{
    std::copy(std::begin(other.m), std::end(other.m), std::begin(m));
}

Matrix3::Matrix3(const Matrix4& other) 
{
    m[0] = other.m[0]; m[3] = other.m[4]; m[6] = other.m[8];
    m[1] = other.m[1]; m[4] = other.m[5]; m[7] = other.m[9];
    m[2] = other.m[2]; m[5] = other.m[6]; m[8] = other.m[10];
}

Matrix3 Matrix3::operator*(float s) const
{
    Matrix3 res;
    for (int i = 0; i < 9; ++i)
        res.m[i] = m[i] * s;
    return res;
}

Vector3 Matrix3::operator*(const Vector3 &v) const
{
    return Vector3(
        v.x * m[0] + v.y * m[3] + v.z * m[6],
        v.x * m[1] + v.y * m[4] + v.z * m[7],
        v.x * m[2] + v.y * m[5] + v.z * m[8]
        );
}

Matrix3 Matrix3::operator*(const Matrix3 &other) const
{
    Matrix3 res;
    for (int i = 0; i < 3; ++i) { // 遍历结果矩阵的行
        for (int j = 0; j < 3; ++j) { // 遍历结果矩阵的列
            float sum = 0.0;
            for (int k = 0; k < 3; ++k) { // 遍历两个矩阵的共同维度
                sum += m[i * 3 + k] * other.m[k * 3 + j]; // 注意索引的计算
            }
            res.m[i * 3 + j] = sum;
        }
    }
    return res;
}

void Matrix3::Set(int row, int col, float t)
{
    assert(row < 3 && col < 3);
    m[col* 3 + row] = t;
}

void Matrix3::Set(float m00, float m01, float m02, 
                  float m10, float m11, float m12, 
                  float m20, float m21, float m22)
{
    m[0] = m00; m[3] = m01; m[6] = m02;
    m[1] = m10; m[4] = m11; m[7] = m12;
    m[2] = m20; m[5] = m21; m[8] = m22;
}

Matrix3 Matrix3::Identity()
{
    Set(1, 0, 0, 
        0, 1, 0, 
        0, 0, 1);
    return *this;
}

Vector3 Matrix3::GetColum(int col) const
{
    assert(col < 3);
    return Vector3(m[col * 3], m[col * 3 + 1], m[col * 3 + 2]);
}

void Matrix3::SetColum(int col, const Vector3 &v)
{
    assert(col < 3);
    m[col * 3] = v.x; 
    m[col * 3 + 1] = v.y; 
    m[col * 3 + 2] = v.z;
}

Matrix4::Matrix4(float v)
{
    std::fill(std::begin(m), std::end(m), 0.0f);
    m[0] = m[5] = m[10] = m[15] = v;
}

Matrix4::Matrix4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    Set(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
}

Matrix4::Matrix4(const Matrix4 &other)
{
    std::copy(std::begin(other.m), std::end(other.m), std::begin(m));
}

Matrix4 Matrix4::operator*(float s) const
{
    Matrix4 res;
    for (int i = 0; i < 16; ++i)
        res.m[i] = m[i] * s;
    return res;
}

Vector4 Matrix4::operator*(const Vector4 &v) const
{
    return Vector4(
        v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12],
        v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13],
        v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14],
        v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15]
        );
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const
{
    Matrix4 res;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 4; ++k) {
                sum += m[i * 4 + k] * other.m[k * 4 + j];
            }
            res.m[i * 4 + j] = sum;
        }
    }
    return res;
}

float Matrix4::Get(int row, int col) const
{
    assert(row < 4 && col < 4);
    return m[col * 4 + row];
}

void Matrix4::Set(int row, int col, float t)
{
    assert(row < 4 && col < 4);
    m[col * 4 + row] = t;
}

void Matrix4::Set(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{
    m[0] = m00; m[4] = m01; m[8] = m02; m[12] = m03;
    m[1] = m10; m[5] = m11; m[9] = m12; m[13] = m13;
    m[2] = m20; m[6] = m21; m[10] = m22; m[14] = m23;
    m[3] = m30; m[7] = m31; m[11] = m32; m[15] = m33;
}

Matrix3 Matrix4::Identity()
{
    Set(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    return *this;
}

Vector4 Matrix4::GetColum(int col) const
{
    assert(col < 4);
    return Vector4(
        m[col * 4],
        m[col * 4 + 1],
        m[col * 4 + 2],
        m[col * 4 + 3]);
}

void Matrix4::SetColum(int col, const Vector4 &v)
{
    assert(col < 4);
    m[col * 4] = v.x;
    m[col * 4 + 1] = v.y;
    m[col * 4 + 2] = v.z;
    m[col * 4 + 3] = v.w;
}

Matrix3 Transpose(const Matrix3 &m)
{
    Matrix3 res;
    auto dest = res.m; 
    auto src = m.m;
    dest[0] = src[0]; dest[3] = src[1]; dest[6] = src[2];
    dest[1] = src[3]; dest[4] = src[4]; dest[7] = src[5];
    dest[2] = src[6]; dest[5] = src[7]; dest[8] = src[8];
    return res;
}

Matrix4 Transpose(const Matrix4 &m)
{
    Matrix4 res;
    auto dest = res.m;
    auto src = m.m;
    dest[0] = src[0];  dest[4] = src[1];  dest[8] =  src[2];  dest[12] = src[3];
    dest[1] = src[4];  dest[5] = src[5];  dest[9] =  src[6];  dest[13] = src[7];
    dest[2] = src[8];  dest[6] = src[9];  dest[10] = src[10]; dest[14] = src[11];
    dest[3] = src[12]; dest[7] = src[13]; dest[11] = src[14]; dest[15] = src[15];
    return res;
}

Matrix4 Inverse(const Matrix4 &src)
{
    Matrix4 res;

    //要求一个矩阵的逆矩阵，首先要求出它的伴随矩阵

    //计算每个必须的2*2矩阵行列式,下标是左上角到右下角
    float D_22_33 = src.Get(2, 2) * src.Get(3, 3) - src.Get(2, 3) * src.Get(3, 2);

    float D_12_23 = src.Get(1, 2) * src.Get(2, 3) - src.Get(1, 3) * src.Get(2, 2);
    float D_12_33 = src.Get(1, 2) * src.Get(3, 3) - src.Get(1, 3) * src.Get(3, 2);

    float D_21_32 = src.Get(2, 1) * src.Get(3, 2) - src.Get(2, 2) * src.Get(3, 1);
    float D_21_33 = src.Get(2, 1) * src.Get(3, 3) - src.Get(2, 3) * src.Get(3, 1);

    float D_11_22 = src.Get(1, 1) * src.Get(2, 2) - src.Get(1, 2) * src.Get(2, 1);
    float D_11_23 = src.Get(1, 1) * src.Get(2, 3) - src.Get(1, 3) * src.Get(2, 1);
    float D_11_32 = src.Get(1, 1) * src.Get(3, 2) - src.Get(1, 2) * src.Get(3, 1);
    float D_11_33 = src.Get(1, 1) * src.Get(3, 3) - src.Get(1, 3) * src.Get(3, 1);

    float D_02_13 = src.Get(0, 2) * src.Get(1, 3) - src.Get(0, 3) * src.Get(1, 2);
    float D_02_23 = src.Get(0, 2) * src.Get(2, 3) - src.Get(0, 3) * src.Get(2, 2);
    float D_02_33 = src.Get(0, 2) * src.Get(3, 3) - src.Get(0, 3) * src.Get(3, 2);

    float D_01_12 = src.Get(0, 1) * src.Get(1, 2) - src.Get(0, 2) * src.Get(1, 1);
    float D_01_13 = src.Get(0, 1) * src.Get(1, 3) - src.Get(0, 3) * src.Get(1, 1);
    float D_01_22 = src.Get(0, 1) * src.Get(2, 2) - src.Get(0, 2) * src.Get(2, 1);
    float D_01_23 = src.Get(0, 1) * src.Get(2, 3) - src.Get(0, 3) * src.Get(2, 1);
    float D_01_32 = src.Get(0, 1) * src.Get(3, 2) - src.Get(0, 2) * src.Get(3, 1);
    float D_01_33 = src.Get(0, 1) * src.Get(3, 3) - src.Get(0, 3) * src.Get(3, 1);

    //计算伴随阵的每列数据
    Vector4 col0, col1, col2, col3;

    col0.x = src.Get(1, 1) * D_22_33 - src.Get(2, 1) * D_12_33 + src.Get(3, 1) * D_12_23;
    col0.y = -(src.Get(1, 0) * D_22_33 - src.Get(2, 0) * D_12_33 + src.Get(3, 0) * D_12_23);
    col0.z = src.Get(1, 0) * D_21_33 - src.Get(2, 0) * D_11_33 + src.Get(3, 0) * D_11_23;
    col0.w = -(src.Get(1, 0) * D_21_32 - src.Get(2, 0) * D_11_32 + src.Get(3, 0) * D_11_22);

    col1.x = -(src.Get(0, 1) * D_22_33 - src.Get(2, 1) * D_02_33 + src.Get(3, 1) * D_02_23);
    col1.y = src.Get(0, 0) * D_22_33 - src.Get(2, 0) * D_02_33 + src.Get(3, 0) * D_02_23;
    col1.z = -(src.Get(0, 0) * D_21_33 - src.Get(2, 0) * D_01_33 + src.Get(3, 0) * D_01_23);
    col1.w = src.Get(0, 0) * D_21_32 - src.Get(2, 0) * D_01_32 + src.Get(3, 0) * D_01_22;

    col2.x = src.Get(0, 1) * D_12_33 - src.Get(1, 1) * D_02_33 + src.Get(3, 1) * D_02_13;
    col2.y = -(src.Get(0, 0) * D_12_33 - src.Get(1, 0) * D_02_33 + src.Get(3, 0) * D_02_13);
    col2.z = src.Get(0, 0) * D_11_33 - src.Get(1, 0) * D_01_33 + src.Get(3, 0) * D_01_13;
    col2.w = -(src.Get(0, 0) * D_11_32 - src.Get(1, 0) * D_01_32 + src.Get(3, 0) * D_01_12);

    col3.x = -(src.Get(0, 1) * D_12_23 - src.Get(1, 1) * D_02_23 + src.Get(2, 1) * D_02_13);
    col3.y = src.Get(0, 0) * D_12_23 - src.Get(1, 0) * D_02_23 + src.Get(2, 0) * D_02_13;
    col3.z = -(src.Get(0, 0) * D_11_23 - src.Get(1, 0) * D_01_23 + src.Get(2, 0) * D_01_13);
    col3.w = src.Get(0, 0) * D_11_22 - src.Get(1, 0) * D_01_22 + src.Get(2, 0) * D_01_12;

    res.SetColum(0, col0);
    res.SetColum(1, col1);
    res.SetColum(2, col2);
    res.SetColum(3, col3);

    //计算行列式
    Vector4 row0(res.Get(0, 0), res.Get(0, 1), res.Get(0, 2), res.Get(0, 3));
    Vector4 colum0 = src.GetColum(0);
    float determinant = Dot(row0, colum0);

    if (determinant == 0) {
        throw std::runtime_error("Inverting a matrix that cannot be inverted");
    }
    
    float oneOverDeterminant = 1.0f / determinant;
    //伴随矩阵 乘上 原矩阵的行列式的倒数 为原矩阵的逆
    return res * oneOverDeterminant;
}

void PrintMatrix(const Matrix3 &matrix)
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << matrix.m[i * 3 + j] << " ";
        }
        std::cout << std::endl;
    }
}

void PrintMatrix(const Matrix4 &matrix)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix.m[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
}
