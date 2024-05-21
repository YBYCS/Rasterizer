#pragma once

#include <windows.h>

// 全局变量声明
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern COLORREF **colorsbuff;
extern HWND g_hwnd;

// 函数声明
void UpdateWindowBuffer(COLORREF **colors);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitializeWindow(HINSTANCE hInstance);
void Tick();

