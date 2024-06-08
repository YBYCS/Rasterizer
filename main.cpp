#include "main.h"
#include <iostream>
#include "Rasterizer.h"
#include "Model.h"
#include <math.h>
#include "Vector.h"
#include "Image.h"
#include "Matrix.h"
#include <memory>

#pragma comment(linker, "/subsystem:window /entry:WinMainCRTStartup")


void Start() {
    // Model model;
    // if (!Model::LoadOBJ("obj/boggie/body.obj", model)) {
    //     std::cout << "打开文件失败" << std::endl;
    // }
    // // 成功加载模型
    // for (int i = 0; i < model.faces.size(); ++i) {
    //     auto face = model.faces[i];
    //     auto p1 = Rasterizer::project(model.vertices[face.vertexIndices[0]]);
    //     auto p2 = Rasterizer::project(model.vertices[face.vertexIndices[1]]);
    //     auto p3 = Rasterizer::project(model.vertices[face.vertexIndices[2]]);
    //     Rasterizer::DrawTriangleEdge(p1, p2, p3);
    // }

    //Rasterizer::DrawTriangle(Point(100, 100, 73, 14, 104, 255), Point(400, 500, 105, 240, 211, 255), Point(700, 100, 255, 0, 0, 255));
    std::unique_ptr<Image> image = Image::CreateImage("assets/picture/Genshin.png");
    Rasterizer::SetBlending(true);
    Rasterizer::SetSamplingMethod(BILINEAR_INTERPOLATION);
    Rasterizer::SetTexture(image.get());
    Point p1(100, 100, Color(255, 0, 0, 0), Vector2(0, 0));
    Point p2(400, 500, Color(0, 255, 0, 0), Vector2(0.5, 1));
    Point p3(700, 100, Color(0, 0, 255, 0), Vector2(1, 0));
    Rasterizer::DrawTriangle(p1, p2, p3);
    window->UpdateWindowBuffer();
}

//主循环 逻辑放这里
void Tick() {
    //首先应该将上一帧绘制内容清空
    //window->Clear();

    //RotateTriangle(0.01f);

    // 更新窗口显示
    window->UpdateWindowBuffer();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {

//    RedirectIOToConsole();
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

float angle = 0.0f;
float cameraPos = 5.0f;

Point p1(0, 0, 255, 0, 0, 255);
Point p2(0, 0, 0, 255, 0, 255);
Point p3(0, 0, 0, 0, 255, 255);

//三角形在世界空间下的坐标
Vector4 pos1(-1, -1, 0, 1);
Vector4 pos2(1, -1, 0, 1);
Vector4 pos3(0, 1, 0, 1);

void RotateTriangle(float amplitude) {
    angle += amplitude;
    cameraPos -= 0.01f;
    //MVP变换和屏幕空间变换矩阵
    Matrix4 modelMatix, viewMatrix, perspectiveMatrix, screenMatrix;
    modelMatix = RotateMartix(Matrix4(), Vector3(0, 1, 0), angle);
    perspectiveMatrix = Perspective(60.0f, 0.1f, 100.0f);
    Matrix4 cameraMatrix = Translate(Matrix4(), Vector3(0, 0, cameraPos));
    viewMatrix = Inverse(cameraMatrix);
    screenMatrix = ScreenMatrix();

    //应用MVP变换
    auto sp1 = perspectiveMatrix * viewMatrix * modelMatix * pos1;
    auto sp2 = perspectiveMatrix * viewMatrix * modelMatix * pos2;
    auto sp3 = perspectiveMatrix * viewMatrix * modelMatix * pos3;

    sp1 /= sp1.w;
    sp2 /= sp2.w;
    sp3 /= sp3.w;

    sp1 = screenMatrix * sp1;
    sp2 = screenMatrix * sp2;
    sp3 = screenMatrix * sp3;

    p1.x = sp1.x;
    p1.y = sp1.y;
    p2.x = sp2.x;
    p2.y = sp2.y;
    p3.x = sp3.x;
    p3.y = sp3.y;

    Rasterizer::DrawTriangle(p1, p2, p3);
}

//屏幕雪花噪点效果
void SnowflakeNoise() {
    for (int i = 0; i < window->GetWidth(); i++) {
        for (int j = 0; j < window->GetHeight(); j++) {
            int v = std::rand() % 255;
            window->DrawPoint(j, i, RGB(v,v,v));
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