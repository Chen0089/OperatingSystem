#include "Include.hh"   // 项目的依赖项声明
#include "define.hh"    // 项目的硬编码定义
#include "DesktopUI.h"  // 项目的DLL声明

// 全局变量
namespace {
    HINSTANCE g_hInstance;
    HWND g_hMainWnd;
    std::atomic<bool> g_running{false};
}

// 窗口过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

// 初始化GDI+
void InitializeGdiPlus() {
    static ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

// 导出函数实现
DESKTOPUI_API void InitializeDesktop() {
    InitializeGdiPlus();
    
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = g_hInstance;
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"DesktopUIWindow";
    RegisterClassEx(&wc);
}

DESKTOPUI_API void CreateMainWindow() {
    g_hMainWnd = CreateWindowEx(
        WS_EX_LAYERED,
        L"DesktopUIWindow",
        L"智能桌面",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr, g_hInstance, nullptr);
    
    SetLayeredWindowAttributes(g_hMainWnd, 0, 225, LWA_ALPHA);
    ShowWindow(g_hMainWnd, SW_SHOW);
}

DESKTOPUI_API void RunMessageLoop() {
    MSG msg;
    g_running = true;
    while (g_running && GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

DESKTOPUI_API void ShutdownDesktop() {
    g_running = false;
    DestroyWindow(g_hMainWnd);
    UnregisterClass(L"DesktopUIWindow", g_hInstance);
}

// 新增导出函数：启动桌面系统
extern "C" DESKTOPUI_API void StartDesktopSystem() {
    InitializeDesktop();
    CreateMainWindow();
    RunMessageLoop();
    ShutdownDesktop();
}

// DLL入口点
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hModule;
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        // 清理资源
        break;
    }
    return TRUE;
}
