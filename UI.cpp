#include "Include.hh"
#include "define.hh"

// 全局变量
namespace {
    HINSTANCE g_hInstance;
    HWND g_hMainWnd;
    WCHAR g_bgPath[MAX_PATH] = L"";
    Image* g_pBgImage = nullptr;
    const int TASKBAR_HEIGHT = 40;
}

// 前向声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadRandomBackground();
void DrawTaskbar(HDC hdc, RECT clientRect);

// DLL导出函数
extern "C" __declspec(dllexport) void CreateDesktopWindow(LPCWSTR imageFolder) {
    // 保存背景路径
    wcsncpy_s(g_bgPath, imageFolder, MAX_PATH);

    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 注册窗口类
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = g_hInstance;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"DesktopWindowClass";
    RegisterClassExW(&wcex);

    // 创建全屏窗口
    g_hMainWnd = CreateWindowExW(
        WS_EX_LAYERED,  // 支持透明度
        L"DesktopWindowClass",
        L"Smart Desktop",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        screenWidth, screenHeight,
        nullptr,
        nullptr,
        g_hInstance,
        nullptr
    );

    // 设置窗口透明度
    SetLayeredWindowAttributes(g_hMainWnd, 0, 255, LWA_ALPHA);

    // 加载随机背景
    LoadRandomBackground();

    // 显示窗口
    ShowWindow(g_hMainWnd, SW_SHOW);
    UpdateWindow(g_hMainWnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 清理资源
    if (g_pBgImage) delete g_pBgImage;
    GdiplusShutdown(gdiplusToken);
}

// 窗口过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        // 双缓冲绘图
        HDC hdcMem = CreateCompatibleDC(hdc);
        RECT rc;
        GetClientRect(hWnd, &rc);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        SelectObject(hdcMem, hbmMem);

        // 绘制背景
        if (g_pBgImage) {
            Graphics graphics(hdcMem);
            graphics.DrawImage(g_pBgImage, 0, 0, rc.right, rc.bottom - TASKBAR_HEIGHT);
        } else {
            FillRect(hdcMem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
        }

        // 绘制任务栏
        DrawTaskbar(hdcMem, rc);

        // 复制到屏幕
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

        // 清理资源
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 加载随机背景图
void LoadRandomBackground() {
    std::vector<std::wstring> imageFiles;
    WIN32_FIND_DATAW findData;
    
    // 构建搜索路径
    wchar_t searchPath[MAX_PATH];
    PathCombineW(searchPath, g_bgPath, L"*.*");

    HANDLE hFind = FindFirstFileW(searchPath, &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // 检查扩展名
                const wchar_t* ext = PathFindExtensionW(findData.cFileName);
                if (wcscmp(ext, L".jpg") == 0 || wcscmp(ext, L".png") == 0) {
                    wchar_t fullPath[MAX_PATH];
                    PathCombineW(fullPath, g_bgPath, findData.cFileName);
                    imageFiles.emplace_back(fullPath);
                }
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }

    // 随机选择图片
    if (!imageFiles.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, imageFiles.size() - 1);
        
        if (g_pBgImage) delete g_pBgImage;
        g_pBgImage = Image::FromFile(imageFiles[dis(gen)].c_str());
    }
}

// 绘制任务栏
void DrawTaskbar(HDC hdc, RECT clientRect) {
    Graphics graphics(hdc);
    
    // 任务栏区域
    Rect taskbarRect(
        0, 
        clientRect.bottom - TASKBAR_HEIGHT,
        clientRect.right,
        TASKBAR_HEIGHT
    );

    // 圆角矩形路径
    GraphicsPath path;
    int cornerRadius = 15;
    path.AddArc(taskbarRect.X, taskbarRect.Y, cornerRadius, cornerRadius, 180, 90);
    path.AddArc(taskbarRect.GetRight() - cornerRadius, taskbarRect.Y, cornerRadius, cornerRadius, 270, 90);
    path.AddArc(taskbarRect.GetRight() - cornerRadius, taskbarRect.GetBottom() - cornerRadius, cornerRadius, cornerRadius, 0, 90);
    path.AddArc(taskbarRect.X, taskbarRect.GetBottom() - cornerRadius, cornerRadius, cornerRadius, 90, 90);
    path.CloseFigure();

    // 渐变画刷
    LinearGradientBrush brush(
        Point(0, taskbarRect.Y),
        Point(0, taskbarRect.GetBottom()),
        Color(150, 240, 240, 240),  // 半透明白色
        Color(220, 200, 200, 200)   // 浅灰色
    );

    // 绘制任务栏
    graphics.FillPath(&brush, &path);

    // 添加系统图标
    HICON hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    DrawIconEx(hdc, 10, taskbarRect.Y + 5, hIcon, 32, 32, 0, nullptr, DI_NORMAL);
}

// DLL入口点
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    g_hInstance = hModule;
    return TRUE;
}
