#pragma once
#include <windows.h>
#include <stdint.h>
#include "Color.h"

#define window WindowController::getInstance()

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class WindowController {
public:
    static WindowController* getInstance();
    WindowController();
    ~WindowController();

    void InitializeWindow(HINSTANCE hInstance);
    // 处理窗口事件
    void HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //捕获并分发窗口消息
    bool MyPeekMessage(MSG& msg);
    void UpdateWindowBuffer();
    void DrawPoint(int x, int y, Color color);
    Color GetColor(int x, int y);
    void Clear();
    int GetWidth() { return width_; };
    int GetHeight() { return height_; };
private:
    void CreateAWindow(HINSTANCE hInstance);
    ATOM RegisterWindowClass(HINSTANCE hInstance);

private:
    static WindowController* instance_;
    bool alive_ = true;  //窗口是否正在工作
    HINSTANCE hInstance_;   //窗口所属的应用程序实例句柄
    WCHAR windowClassName_[100] = L"Sample Window Class";   //窗口类的名称，通过名称来唯一表示窗口类
    HWND hwnd_; //窗口实例句柄
    int width_ = 800;
    int height_ = 600;
    HDC hdc_;                           //前台缓冲区内存设备上下文句柄
    HDC memDC_;                         //后台缓冲区内存设备上下文句柄
    HBITMAP hBitmap_;                   //位图
    void* graphicsBuffer_ = nullptr;    //指向位图像素数据的指针
    Color* colorBuffer_ = nullptr;      //实际用于操作画布颜色的内存
};