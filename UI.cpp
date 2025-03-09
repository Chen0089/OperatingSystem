#include "Include.hh"           // 依赖项的include
#include "define.hh"            // define声明

/*
 * 你可以在这里声明全局变量
 * 但你必须遵循规则
 * 详见codingrule.md
*/
// 这里啥都没有
// Never gonna give you up（你被骗了）

// 简单的窗口消息处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(
		    	hWnd,
		    	message,
		    	wParam,
		    	lParam
			);
    }
    return 0;
}

// 导出的窗口创建函数
extern "C" __declspec(dllexport) void CreateDesktopWindow() {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "DesktopWindowClass";
    RegisterClassA(&wc);

    HWND hWnd = CreateWindowA(
        "DesktopWindowClass",
	
        "Desktop App",
	
        WS_OVERLAPPEDWINDOW,
	
        CW_USEDEFAULT,
		CW_USEDEFAULT,
	
        800,
		600,
	
        NULL,
		NULL,
	
        GetModuleHandle(NULL),
	
        NULL
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    // 消息循环
    MSG msg;
    while (
		GetMessage(
			&msg,
			NULL,
			0,
			0
		)
	) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// DLL入口点
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
) {
    return TRUE;
}
