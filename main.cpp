#include "main.h"
#include <windows.h>
#include <iostream>
#include "Rasterizer.h"
#include "Model.h"

#pragma comment(linker, "/subsystem:window /entry:WinMainCRTStartup")
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
COLORREF **colorsbuff = nullptr;
HWND g_hwnd = NULL;
//int main(){
//    std::cout<<"hello"<<std::endl;
//    return 0;
//}


void UpdateWindowBuffer(COLORREF **colors) {
    if (!g_hwnd)
        return;

    HDC hdc = GetDC(g_hwnd);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
    SelectObject(memDC, hBitmap);

    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WINDOW_WIDTH;
    bmi.bmiHeader.biHeight = -WINDOW_HEIGHT; // top-down when negative
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    COLORREF *flatArray = new COLORREF[WINDOW_WIDTH * WINDOW_HEIGHT];
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        for (int x = 0; x < WINDOW_WIDTH; ++x) {
            flatArray[(WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x] = colors[y][x];
        }
    }

    SetDIBits(memDC, hBitmap, 0, WINDOW_HEIGHT, flatArray, &bmi, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);

    delete[] flatArray;
    DeleteObject(hBitmap);
    DeleteDC(memDC);
    ReleaseDC(g_hwnd, hdc);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void InitializeWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    g_hwnd = CreateWindowEx(0, CLASS_NAME, "Window Title", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
                            WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(g_hwnd, SW_SHOW);
}

void Start() {
//    Rasterizer::DrawTriangle(Point(0,0,0,0,0,255),Point(0,200,200,200,200,255),Point(200,200,0,0,222,255));
    Model model;
    if (Model::loadOBJ("obj/boggie/body.obj", model)) {
        // 成功加载模型

        for (int i = 0; i < model.faces.size(); ++i) {
            auto face = model.faces[i];
            auto p1 = Rasterizer::project(model.vertices[face.vertexIndices[0]]);
            auto p2 = Rasterizer::project(model.vertices[face.vertexIndices[1]]);
            auto p3 = Rasterizer::project(model.vertices[face.vertexIndices[2]]);
            Rasterizer::DrawTriangleEdge(p1,p2,p3);
        }
        

    } else {
        std::cout << "打开文件失败" << std::endl;
    }

    UpdateWindowBuffer(colorsbuff);

}

//主循环 逻辑放这里
void Tick() {
    //todo 执行渲染逻辑
    //Rasterizer::DrawLine(Point(0, 0, Color(2, 3, 3)), Point(55, 55, Color(233, 233, 233)));
    // 更新窗口显示
    UpdateWindowBuffer(colorsbuff);
}

// 显示一个打印窗口
void RedirectIOToConsole() {
    HWND consoleWindow = GetConsoleWindow();

    if (consoleWindow == NULL) {
        // 没有控制台窗口，尝试分配一个新的控制台
        if (!AllocConsole()) {
            MessageBox(NULL, "Failed to allocate console!", "Error", MB_OK | MB_ICONERROR);
            return;
        }
    } else {
        // 已有控制台窗口，显示并聚焦
        ShowWindow(consoleWindow, SW_SHOW);
        SetForegroundWindow(consoleWindow);
    }

    FILE *fp;
    if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0 || fp == NULL) {
        MessageBox(NULL, "Failed to redirect stdout!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0 || fp == NULL) {
        MessageBox(NULL, "Failed to redirect stderr!", "Error", MB_OK | MB_ICONERROR);
        return;
    }
    if (freopen_s(&fp, "CONIN$", "r", stdin) != 0 || fp == NULL) {
        MessageBox(NULL, "Failed to redirect stdin!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    consoleWindow = GetConsoleWindow();
    if (consoleWindow) {
        ShowWindow(consoleWindow, SW_SHOW);
        SetForegroundWindow(consoleWindow);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

//    RedirectIOToConsole();
    std::cout << "hello world" << std::endl;
    //初始化buff
    colorsbuff = new COLORREF *[WINDOW_HEIGHT];
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        colorsbuff[y] = new COLORREF[WINDOW_WIDTH];
    }

    InitializeWindow(hInstance);
    Start();
    // Main message loop
    MSG msg = {};
    int i = 155;
    //Tick 循环
    while (true) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
            break;

        //创建颜色
        // for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        //   for (int x = 0; x < WINDOW_WIDTH; ++x) {
        //     colorsbuff[y][x] = RGB(i+x % 256, i*2+y % 256, i*3+x+y % 256); // 生成颜色
        //   }
        // }

        // i++;

        Tick();
    }
    return 0;
}
