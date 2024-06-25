#include "WindowController.h"
#include "main.h"
#include <iostream>
#include "Render.h"

WindowController* WindowController::instance_ = nullptr;
WindowController* WindowController::getInstance() 
{
    if (instance_ == nullptr) {
        instance_ = new WindowController(); 
    }
    return instance_;
}

WindowController::WindowController() {}
WindowController::~WindowController() 
{
    if (hBitmap_) DeleteObject(hBitmap_);
    if (memDC_) DeleteDC(memDC_);
    if (hdc_) ReleaseDC(hwnd_, hdc_);
    if (colorBuffer_) delete[] colorBuffer_;
}

void WindowController::InitializeWindow(HINSTANCE hInstance) 
{
    RegisterWindowClass(hInstance);
    CreateAWindow(hInstance);

    // 初始化 HDC 和 HBITMAP
    hdc_ = GetDC(hwnd_);    //获取指定窗口的设备上下文(DC)
    memDC_ = CreateCompatibleDC(hdc_);  //创建一个与指定设备上下文兼容的内存设备上下文(DC)

    // 设置位图信息
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width_;
    bmi.bmiHeader.biHeight = height_;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;  //设置每个像素的位数，这里设置为32位意味着每个像素包含4个字节
    bmi.bmiHeader.biCompression = BI_RGB;

    // CreateDIBSection 会根据bmi指定的位图信息创建并返回一个设备无关的位图，并为其像素数据分配内存
    //分配的内存地址被设置到 graphicsBuffer_ 指针中，可以通过这个指针直接访问和修改位图的像素数据
    //其中，DIB_RGB_COLORS表示颜色信息以RGB格式存储
    hBitmap_ = CreateDIBSection(memDC_, &bmi, DIB_RGB_COLORS, (void**)&graphicsBuffer_, 0, 0);
    SelectObject(memDC_, hBitmap_);
    memset(graphicsBuffer_, 0, width_ * height_ * 4);   //清空位图像素数据
    colorBuffer_ = (Color*)graphicsBuffer_;
    //初始化深度图
    Render::InitializeDepthMap();
}

ATOM WindowController::RegisterWindowClass(HINSTANCE hInstance) 
{
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);         //函数会检查 cbSize 的值来确保你传递的结构体是正确的类型和大小。如果 cbSize 的值不正确，这些函数可能会失败，导致窗口类注册不成功。
    wc.style = CS_HREDRAW | CS_VREDRAW;     //设置当水平或垂直大小发生变化时重绘窗口
    wc.lpfnWndProc = WindowProc;            //设置窗口事件回调函数
    wc.hInstance = hInstance;               //设置窗口类所属应用程序的句柄
    wc.lpszClassName = windowClassName_;    //设置窗口类的名称

    return RegisterClassExW(&wc);
}

void WindowController::CreateAWindow(HINSTANCE hInstance) 
{
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
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    WindowController::getInstance()->HandleMessage(hwnd, uMsg, wParam, lParam);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool WindowController::MyPeekMessage(MSG& msg) 
{
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return alive_;
}

void WindowController::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);    //此处销毁窗体会自动发出WM_DESTROY
            break;
        case WM_PAINT:              //窗口需要重绘时
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                Tick();
                EndPaint(hwnd, &ps);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);     //发出线程终止请求
            exit(0);                //退出进程
            break;
    }
}

void WindowController::UpdateWindowBuffer() 
{
    if (!hwnd_) return;

    //将内存设备上下文的内容复制到窗口设备上下文
    BitBlt(hdc_, 0, 0, width_, height_, memDC_, 0, 0, SRCCOPY);
}

void WindowController::DrawPoint(int x, int y, Color color) 
{
    if (x == width_ || y == height_) return;

    Color res = color;
    int index = y * width_ + x;
    if (Render::IsBlendingEnabled()) {
        Color originalColor = colorBuffer_[index];
        float weight = static_cast<float>(color.a) / 255.0f;
        
        res.r = color.r * weight + originalColor.r * (1.0f - weight);
        res.g = color.g * weight + originalColor.g * (1.0f - weight);
        res.b = color.b * weight + originalColor.b * (1.0f - weight);
        res.a = color.a * weight + originalColor.a * (1.0f - weight);
    }
    colorBuffer_[index] = res;
}

/// @brief 得到当前窗口指定位置的颜色
/// @param x 
/// @param y 
/// @return 
Color WindowController::GetColor(int x, int y)
{
    if (x == width_ || y == height_) return Color();
    return colorBuffer_[y * width_ + x];
}

void WindowController::Clear() 
{
    std::fill_n(colorBuffer_, width_ * height_, Color(0, 0, 0, 0));
    //重新设置深度测试中深度图的深度值
    Render::InitializeDepthMap();
}