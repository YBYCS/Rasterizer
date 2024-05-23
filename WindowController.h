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

    void InitializeWindow(HINSTANCE hInstance, const uint32_t& width = 800, const uint32_t& height = 600);

    // 处理窗口事件
    void HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    //捕获并分发窗口消息
    bool MyPeekMessage(MSG& msg);

    void UpdateWindowBuffer();

    void SetColorsbuff(int x, int y, unsigned long color);
private:
    void CreateAWindow(HINSTANCE hInstance);
    ATOM RegisterWindowClass(HINSTANCE hInstance);

private:
    static WindowController* instance_;
    bool alive_ { true } ;  //窗口是否正在工作
    HINSTANCE hInstance_;   //窗口所属的应用程序实例句柄
    WCHAR windowClassName_[100] = L"Sample Window Class";   //窗口类的名称，通过名称来唯一表示窗口类
    HWND hwnd_; //窗口实例句柄
    int width_ = 800;
    int height_ = 600;
    COLORREF **colorsbuff_ = nullptr;   //前台缓冲区
    HDC hdc_;                           //前台缓冲区内存设备上下文句柄
    COLORREF *flatArray_ = nullptr;     //后台缓冲区
    HDC memDC_;                         //后台缓冲区内存设备上下文句柄
    HBITMAP hBitmap_;                   //位图
};