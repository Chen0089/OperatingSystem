#include "Include.hh"
#include "define.hh"

// 全局变量
namespace {
    HINSTANCE g_hInstance;
    HWND g_hMainWnd;
    const int TASKBAR_HEIGHT = 40;
    const int STATUS_RIGHT_MARGIN = 20;
    ComPtr<INetworkListManager> g_networkManager;
}

// 前向声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSystemStatus(HDC hdc, RECT clientRect);
bool CheckNetworkStatus();

// DLL导出函数
extern "C" __declspec(dllexport) void CreateDesktopWindow(LPCWSTR imageFolder) {
    // 初始化COM
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    
    // 创建网络管理器实例
    CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL,
                     IID_PPV_ARGS(&g_networkManager));

    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // 注册窗口类
    WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = g_hInstance;
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
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
        nullptr, nullptr, g_hInstance, nullptr
    );

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
    CoUninitialize();
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

        // 绘制背景（原有逻辑）
        // ...

        // 绘制系统状态
        DrawSystemStatus(hdcMem, rc);

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

// 绘制系统状态信息
void DrawSystemStatus(HDC hdc, RECT clientRect) {
    Graphics graphics(hdc);
    
    // 状态栏区域（右上角）
    const int statusHeight = 30;
    RectF statusRect(
        clientRect.right - 300, // 宽度预留300px
        10,
        280,
        statusHeight
    );

    // 创建渐变背景
    LinearGradientBrush statusBrush(
        PointF(statusRect.X, statusRect.Y),
        PointF(statusRect.GetRight(), statusRect.GetBottom()),
        Color(180, 50, 50, 50),   // 半透明深灰
        Color(180, 100, 100, 100) // 浅灰
    );
    
    // 圆角矩形
    GraphicsPath path;
    int radius = 8;
    path.AddArc(statusRect.X, statusRect.Y, radius, radius, 180, 90);
    path.AddArc(statusRect.GetRight() - radius, statusRect.Y, radius, radius, 270, 90);
    path.AddArc(statusRect.GetRight() - radius, statusRect.GetBottom() - radius, radius, radius, 0, 90);
    path.AddArc(statusRect.X, statusRect.GetBottom() - radius, radius, radius, 90, 90);
    path.CloseFigure();
    graphics.FillPath(&statusBrush, &path);

    // 设置字体
    Font font(L"Arial", 12, FontStyleBold);
    SolidBrush textBrush(Color(240, 240, 240));

    // 获取当前时间
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    WCHAR timeStr[64];
    swprintf_s(timeStr, L"%02d:%02d:%02d", 
              sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

    // 获取网络状态
    bool isOnline = CheckNetworkStatus();
    const WCHAR* netStatus = isOnline ? L"● Online" : L"○ Offline";
    Color netColor = isOnline ? Color(100, 255, 100) : Color(255, 100, 100);

    // 绘制网络状态指示器
    SolidBrush netBrush(netColor);
    graphics.FillEllipse(&netBrush, 
        statusRect.GetRight() - 50, 
        statusRect.Y + 10, 
        10, 10);

    // 布局文本
    StringFormat format;
    format.SetAlignment(StringAlignmentNear);
    format.SetLineAlignment(StringAlignmentCenter);

    // 绘制时间
    graphics.DrawString(
        timeStr, -1, &font,
        RectF(statusRect.X + 10, statusRect.Y, 100, statusRect.Height),
        &format, &textBrush);

    // 绘制网络状态
    graphics.DrawString(
        netStatus, -1, &font,
        RectF(statusRect.GetRight() - 120, statusRect.Y, 110, statusRect.Height),
        &format, &textBrush);
}

// 检测网络状态
bool CheckNetworkStatus() {
    VARIANT_BOOL isConnected = VARIANT_FALSE;
    if (g_networkManager) {
        g_networkManager->get_IsConnectedToInternet(&isConnected);
    }
    return isConnected == VARIANT_TRUE;
}

// DLL入口点
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        g_hInstance = hModule;
        DisableThreadLibraryCalls(hModule); // 优化DLL性能
    }
    return TRUE;
}
