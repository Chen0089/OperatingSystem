#include "Include.hh"
#include "define.hh"

// 全局变量
namespace {
    HINSTANCE g_hInstance;
    HWND g_hMainWnd;
    const int TASKBAR_HEIGHT = 40;
    const int STATUS_RIGHT_MARGIN = 20;
    std::wstring g_currentPath = L"C:\\"; // 默认路径
    std::vector<std::wstring> g_files;
    std::vector<std::wstring> g_folders;
}

// 前向声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSystemStatus(HDC hdc, RECT clientRect);
void DrawFileExplorer(HDC hdc, RECT clientRect);
void LoadDirectoryContents();
void EnableAcrylicBlur(HWND hWnd, COLORREF tintColor = RGB(100, 100, 100), BYTE opacity = 75);

// 主函数
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    g_hInstance = hInstance;

    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // 注册窗口类
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"EnhancedDesktopClass";
    RegisterClassExW(&wcex);

    // 创建全屏窗口
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    g_hMainWnd = CreateWindowExW(
        WS_EX_LAYERED,
        L"EnhancedDesktopClass",
        L"Smart Desktop",
        WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight,
        nullptr, nullptr, hInstance, nullptr
    );

    // 启用亚克力模糊效果
    EnableAcrylicBlur(g_hMainWnd, RGB(100, 100, 100), 75);

    // 设置定时器（每秒更新）
    SetTimer(g_hMainWnd, 1, 1000, nullptr);

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
    KillTimer(g_hMainWnd, 1);
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
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

        // 绘制背景（亚克力模糊已启用）
        DrawFileExplorer(hdcMem, rc);

        BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

        // 清理资源
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, nullptr, FALSE); // 触发重绘
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 启用亚克力模糊效果
void EnableAcrylicBlur(HWND hWnd, COLORREF tintColor, BYTE opacity) {
    // 设置窗口属性
    BOOL useDarkMode = TRUE;
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

    // 亚克力效果参数
    ACCENT_POLICY accent = {
        ACCENT_ENABLE_ACRYLICBLURBEHIND, // 启用亚克力模糊
        0, // 保留位
        (tintColor & 0x00FFFFFF) | (opacity << 24), // ARGB颜色
        0
    };

    WINDOWCOMPOSITIONATTRIBDATA data = {
        WCA_ACCENT_POLICY,
        &accent,
        sizeof(accent)
    };

    // 应用模糊效果
    SetWindowCompositionAttribute(hWnd, &data);
}

// 加载目录内容
void LoadDirectoryContents() {
    g_files.clear();
    g_folders.clear();

    std::wstring searchPath = g_currentPath + L"\\*";
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0) {
                    g_folders.push_back(findData.cFileName);
                }
            } else {
                g_files.push_back(findData.cFileName);
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }
}

// 绘制文件资源管理器
void DrawFileExplorer(HDC hdc, RECT clientRect) {
    Graphics graphics(hdc);

    // 绘制背景（亚克力模糊已启用）
    SolidBrush bgBrush(Color(75, 100, 100, 100)); // 半透明灰色
    graphics.FillRectangle(&bgBrush, 0, 0, clientRect.right, clientRect.bottom);

    // 绘制文件列表
    int y = 50;
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                             OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, L"Arial");
    SelectObject(hdc, hFont);

    // 绘制文件夹
    for (const auto& folder : g_folders) {
        TextOutW(hdc, 10, y, folder.c_str(), folder.length());
        y += 20;
    }

    // 绘制文件
    for (const auto& file : g_files) {
        TextOutW(hdc, 10, y, file.c_str(), file.length());
        y += 20;
    }

    DeleteObject(hFont);
}
