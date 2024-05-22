#include "WindowController.h"
#include<iostream>

WindowController* WindowController::instance_ = nullptr;
WindowController* WindowController::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new WindowController(); 
    }
    return instance_;
}

WindowController::WindowController() {}
WindowController::~WindowController() {}

void WindowController::InitializeWindow(HINSTANCE hInstance, const uint32_t& width, const uint32_t& height) {
    width_ = width;
    height_ = height;

    //初始化buff
    colorsbuff_ = new COLORREF *[height_];
    for (int y = 0; y < height_; ++y) {
        colorsbuff_[y] = new COLORREF[width_];
    }
    RegisterWindowClass(hInstance);
    CreateAWindow(hInstance);
}

ATOM WindowController::RegisterWindowClass(HINSTANCE hInstance) {
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);         //函数会检查 cbSize 的值来确保你传递的结构体是正确的类型和大小。如果 cbSize 的值不正确，这些函数可能会失败，导致窗口类注册不成功。
    wc.style = CS_HREDRAW | CS_VREDRAW;     //设置当水平或垂直大小发生变化时重绘窗口
    wc.lpfnWndProc = WindowProc;            //设置窗口事件回调函数
    wc.hInstance = hInstance;               //设置窗口类所属应用程序的句柄
    wc.lpszClassName = windowClassName_;    //设置窗口类的名称

    return RegisterClassExW(&wc);
}

void WindowController::CreateAWindow(HINSTANCE hInstance) {
    hInstance_ = hInstance;

    long dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
    //计算实际显示区域的大小，这个大小不包括窗体中不用于显示的部分如标题栏等
    RECT windowRect;
    windowRect.left = 0L;
    windowRect.top = 0L;
    windowRect.right = (long)width_;
    windowRect.bottom = (long)height_;
    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, WS_EX_APPWINDOW);

    hwnd_ = CreateWindowExW(
        0,                          //扩展窗口样式
        windowClassName_,           //窗口类的名称
        (LPCWSTR)"Genshin Impact",  //窗口标题
        dwStyle,                    //窗口样式
        CW_USEDEFAULT,              //窗口的初始x坐标（CW_USEDEFAULT表示使用默认值），相对于屏幕的左上角
        CW_USEDEFAULT,              //窗口的初始y坐标（CW_USEDEFAULT表示使用默认值）
        windowRect.right - windowRect.left,              //窗口的宽度
        windowRect.bottom - windowRect.top,              //窗口的高度
        NULL,                       //父窗口句柄，NULL表示没有父窗口
        NULL,                       //菜单句柄，NULL表示没有菜单
        hInstance,                  //应用程序实例句柄
        NULL                        //指向窗口创建数据的指针，这里没有使用
    );

    ShowWindow(hwnd_, SW_SHOW);
}

// 窗口事件回调函数，用于处理窗口接收到的消息
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowController::getInstance()->HandleMessage(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool WindowController::MyPeekMessage(MSG& msg) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return alive_;
}

void WindowController::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);    //此处销毁窗体会自动发出WM_DESTROY
            break;
        case WM_PAINT:              //当窗体尺寸发生变化时，需要重绘
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                UpdateWindowBuffer();
                EndPaint(hwnd, &ps);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);     //发出线程终止请求
            exit(0);                //退出进程
            break;
    }
}

void WindowController::UpdateWindowBuffer() {
    if (!hwnd_)
        return;
    HDC hdc = GetDC(hwnd_);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width_, height_);
    SelectObject(memDC, hBitmap);

    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width_;
    bmi.bmiHeader.biHeight = -height_; // top-down when negative
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    COLORREF *flatArray = new COLORREF[width_ * height_];
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            flatArray[(height_ - y - 1) * width_ + x] = colorsbuff_[y][x];
        }
    }

    SetDIBits(memDC, hBitmap, 0, height_, flatArray, &bmi, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, width_, height_, memDC, 0, 0, SRCCOPY);

    delete[] flatArray;
    DeleteObject(hBitmap);
    DeleteDC(memDC);
    ReleaseDC(hwnd_, hdc);
}

void WindowController::SetColorsbuff(int x, int y, unsigned long color) {
    colorsbuff_[x][y] = color;
}