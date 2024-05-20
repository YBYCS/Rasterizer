#include <windows.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
HWND g_hwnd = NULL;

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  InitializeWindow(hInstance);

  // Main message loop
  MSG msg = {};
  int i = 155;
  while (true) {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    if (msg.message == WM_QUIT)
      break;
    // 创建颜色数组
    COLORREF **colors = new COLORREF *[WINDOW_HEIGHT];
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
      colors[y] = new COLORREF[WINDOW_WIDTH];
      for (int x = 0; x < WINDOW_WIDTH; ++x) {
        colors[y][x] = RGB(i % 256, i % 256, i % 256); // 生成颜色
      }
    }
    i++;
    // 更新窗口显示
    UpdateWindowBuffer(colors);

    // 清理颜色数组
    for (int y = 0; y < WINDOW_HEIGHT; ++y) {
      delete[] colors[y];
    }
    delete[] colors;
  }
  return 0;
}
