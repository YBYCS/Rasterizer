#pragma once

#include "Vector.h"
#include "Matrix.h"

class Camera {
private:
    Vector3 position_ { 0.0f, 0.0f, 2.5f };      //摄像机当前位置
    Vector3 front_ { 0.0f, 0.0f, -1.0f };        //摄像机看向的方向，默认看向-z方向
    Vector3 worldUp_ { 0.0f, 1.0f, 0.0f };       //摄像机上方向
    float movementSpeed_ = 0.01f;                //鼠标左键移动摄像机时，摄像机的移动速度
    float rotationSpeed_ = 0.1f;                //鼠标右键旋转摄像机时，摄像机的旋转速度
    float yaw_ = -90.0f;   //仰角
    float pitch_ = 0.0f;   //摄像机绕Y轴旋转角度

    float curMouseX = 0.0f;
    float curMouseY = 0.0f;
    bool isLeftMouseButtonPressed_ = false;
    bool isRightMouseButtonPressed_ = false;
private:
    Matrix4 viewMatrix_;
    Matrix4 projectionMatrix_;  //投影矩阵也由摄像机类进行计算
public:
    Camera(float fovy, float n, float f, const Vector3& top);
    ~Camera();

    Matrix4 GetViewMatrix() { return viewMatrix_; };
    Matrix4 GetProjectionMatrix() { return projectionMatrix_; };

    void OnLeftMouseDown(int x, int y);
    void OnRightMouseDown(int x, int y);
    void OnLeftMouseUp();
    void OnRightMouseUp();
    void OnMouseMove(int x, int y);
};