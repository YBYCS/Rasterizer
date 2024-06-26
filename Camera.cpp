#include "Camera.h"
#include "WindowController.h"

#define DEG2RAD(theta)		(0.01745329251994329 * (theta))

/// @brief 摄像机类构造函数
/// @param fovy 摄像机FOV
/// @param n 视锥体近平面到摄像机的距离
/// @param f 视锥体远平面到摄像机的距离
/// @param top 摄像机上方向
Camera::Camera(float fovy, float n, float f, const Vector3 &top)
{
    worldUp_ = top;
    //计算投影矩阵
    projectionMatrix_ = Perspective(fovy, n, f, (float)window->GetWidth() / (float)window->GetHeight());
    //计算视图矩阵
    viewMatrix_ = LookAt(position_, position_ + front_, worldUp_);
}

Camera::~Camera()
{
}

void Camera::OnLeftMouseDown(int x, int y)
{
    if (isLeftMouseButtonPressed_) 
        return;
    curMouseX = x;
    curMouseY = y;
    isLeftMouseButtonPressed_ = true;
}

void Camera::OnRightMouseDown(int x, int y)
{
    if (isLeftMouseButtonPressed_) 
        return;
    curMouseX = x;
    curMouseY = y;
    isRightMouseButtonPressed_ = true;
}

void Camera::OnLeftMouseUp()
{
    isLeftMouseButtonPressed_ = false;
}

void Camera::OnRightMouseUp()
{
    isRightMouseButtonPressed_ = false;
}

void Camera::OnMouseMove(int x, int y)
{
    if (isLeftMouseButtonPressed_) {
        //计算这一帧与上一帧之间鼠标的位移量
        int xOffset = curMouseX - x;
        int yOffset = y - curMouseY;

        Vector3 right_ = Normalize(Cross(front_, worldUp_)); //摄像机右轴，即摄像机空间的x轴的正方向
        Vector3 up_ = Normalize(Cross(right_, front_));      //摄像机上轴，即摄像机空间的y轴的正方向
        position_ += Normalize(right_ * xOffset + up_ * yOffset) * movementSpeed_;

        viewMatrix_ = LookAt(position_, position_ + front_, worldUp_);
    } else if (isRightMouseButtonPressed_) {
        //上下旋转
        pitch_ += (curMouseY - y) * rotationSpeed_;
        //为了简单起见，避免反转，否则会出现一些奇怪的问题。
        if (pitch_ > 89.0f) pitch_ = 89.0f;
        else if (pitch_ < -89.0f) pitch_ = -89.0f;
        
        //左右旋转
        yaw_ += (x - curMouseX) * rotationSpeed_;
        //旋转后再次计算摄像机朝向
        front_.x = cos(DEG2RAD(yaw_)) * cos(DEG2RAD(pitch_));
        front_.y = sin(DEG2RAD(pitch_));
        front_.z = sin(DEG2RAD(yaw_)) * cos(DEG2RAD(pitch_));
        front_ = Normalize(front_);

        //记录鼠标位置
        curMouseX = x;
        curMouseY = y;

        viewMatrix_ = LookAt(position_, position_ + front_, worldUp_);
    }
}