#include "main.h"
#include <iostream>
#include "Rasterizer.h"
#include "Model.h"
#include <math.h>
#include "Vector.h"
#include "Image.h"
#include "Matrix.h"
#include <memory>
#include "Render.h"
#include "Simpleshader.h"
#include "HalfLambertShader.h"

#pragma comment(linker, "/subsystem:window /entry:WinMainCRTStartup")

Matrix4 modelMatrix;
Matrix4 viewMatrix;
Matrix4 perspectiveMatrix;

void Start() {
    //计算MVP矩阵
    perspectiveMatrix = Perspective(60.0f, 0.1f, 100.0f);
    //模拟相机位置
    auto cameraModelMatrix = Translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, 2.0f));
    viewMatrix = Inverse(cameraModelMatrix);
    modelMatrix = Matrix4();
    //设置应用哪一个shader
    HalfLambertShader *shader = new HalfLambertShader();
    shader->SetModelMatrix(modelMatrix);
    shader->SetViewMatrix(viewMatrix);
    shader->SetProjectMatrix(perspectiveMatrix);
    Render::SetShader(shader);
    shader->ambientLight.color = Vector3(1.0f, 1.0f, 1.0f);
    shader->ambientLight.intensity = 0.1f;
    shader->directionalLight.color = Vector3(1.0f, 1.0f, 1.0f);
    shader->directionalLight.direction = Vector3(-1.0f, -0.3f, -0.7f);
    shader->directionalLight.intensity = 1.4f;

    //读取模型
    Model model;
    if(!Model::LoadOBJ("assets/obj/african_head/african_head.obj", model)) {
        std::cout << "fuck";
        return;
    }
    //读取纹理
    std::unique_ptr<Image> textureImage = Image::CreateImage("assets/obj/african_head/african_head_diffuse.tga");
    Render::RenderModel(model, textureImage.get());
    window->UpdateWindowBuffer();
    delete shader;
}

//主循环 逻辑放这里
void Tick() {
    //首先应该将上一帧绘制内容清空
    //window->Clear();

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

// void RotateTriangle(float amplitude) {
//     angle += amplitude;
//     cameraPos -= 0.01f;
//     //MVP变换和屏幕空间变换矩阵
//     Matrix4 modelMatix, viewMatrix, perspectiveMatrix, screenMatrix;
//     modelMatix = RotateMartix(Matrix4(), Vector3(0, 1, 0), angle);
//     perspectiveMatrix = Perspective(60.0f, 0.1f, 100.0f);
//     Matrix4 cameraMatrix = Translate(Matrix4(), Vector3(0, 0, cameraPos));
//     viewMatrix = Inverse(cameraMatrix);
//     screenMatrix = ScreenMatrix();

//     //应用MVP变换
//     auto sp1 = perspectiveMatrix * viewMatrix * modelMatix * pos1;
//     auto sp2 = perspectiveMatrix * viewMatrix * modelMatix * pos2;
//     auto sp3 = perspectiveMatrix * viewMatrix * modelMatix * pos3;

//     sp1 /= sp1.w;
//     sp2 /= sp2.w;
//     sp3 /= sp3.w;

//     sp1 = screenMatrix * sp1;
//     sp2 = screenMatrix * sp2;
//     sp3 = screenMatrix * sp3;

//     p1.x = sp1.x;
//     p1.y = sp1.y;
//     p2.x = sp2.x;
//     p2.y = sp2.y;
//     p3.x = sp3.x;
//     p3.y = sp3.y;

//     Rasterizer::DrawTriangle(p1, p2, p3);
// }

//屏幕雪花噪点效果
void SnowflakeNoise() {
    for (int i = 0; i < window->GetWidth(); i++) {
        for (int j = 0; j < window->GetHeight(); j++) {
            int v = std::rand() % 255;
            window->DrawPoint(j, i, RGB(v,v,v));
        }
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