#include "main.h"
#include <iostream>
#include "Rasterizer.h"
#include "Model.h"
#include <math.h>

#pragma comment(linker, "/subsystem:window /entry:WinMainCRTStartup")

void Start() {
//    Rasterizer::DrawTriangle(Point(0,0,0,0,0,255),Point(0,200,200,200,200,255),Point(200,200,0,0,222,255));
    // Model model;
    // if (Model::LoadOBJ("obj/boggie/body.obj", model)) {
    //     // 成功加载模型

    //     for (int i = 0; i < model.faces.size(); ++i) {
    //         auto face = model.faces[i];
    //         auto p1 = Rasterizer::project(model.vertices[face.vertexIndices[0]]);
    //         auto p2 = Rasterizer::project(model.vertices[face.vertexIndices[1]]);
    //         auto p3 = Rasterizer::project(model.vertices[face.vertexIndices[2]]);
    //         Rasterizer::DrawTriangleEdge(p1,p2,p3);
    //     }

    // } else {
    //     std::cout << "打开文件失败" << std::endl;
    // }
    window->UpdateWindowBuffer();
}

//主循环 逻辑放这里
void Tick() {
    //首先应该将上一帧绘制内容清空
    window->Clear();

    //todo 执行渲染逻辑
    DrawLineEffect();

    // 更新窗口显示
    window->UpdateWindowBuffer();
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
    window->InitializeWindow(hInstance);
    Start();

    // Main message loop
    MSG msg = {};

    //Tick 循环
    bool alive = true;
    while (alive) {
        alive = window->MyPeekMessage(msg);
        Tick();
    }
    return 0;
}

//屏幕雪花噪点效果
void SnowflakeNoise() {
    for (int i = 0; i < window->GetWidth(); i++) {
        for (int j = 0; j < window->GetHeight(); j++) {
            int v = std::rand() % 255;
            window->SetColorsbuff(j, i, RGB(v,v,v));
        }
    }
}

void DrawLineEffect() {
    int r = 150;
    Point c { 400, 300, Color(255, 0, 0, 255) };
    for (float i = 0; i < 360; i++) {
        float radian = i * 3.14159265358979323846264338f / 180.0f;
        int x = r * sin(radian) + c.x;
        int y = r * cos(radian) + c.y;
        Point p { x, y, Color(rand() % 255, rand() % 255, rand() % 255) };
        Rasterizer::DrawLine(c, p);
    }
}