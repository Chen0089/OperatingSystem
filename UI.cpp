#include "Include.hh"           // 依赖项的include
#include "define.hh"            // define声明

/*
 * 你可以在这里声明全局变量
 * 但你必须遵循规则
 * 详见codingrule.md
*/
// 这里啥都没有
// Never gonna give you up（你被骗了）

// 在cpp中直接实现窗口功能
#include <windows.h>

void CreateDesktopWindow() {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = 
		[]
		(
			HWND hWnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam
		)
		->
		LRESULT {
        	if (msg == WM_DESTROY) {
            	PostQuitMessage(0);
            	return 0;
        	}
        	return DefWindowProcA(
				hWnd,
				msg,
				wParam,
				lParam
			);
    	};
    	wc.hInstance = GetModuleHandle(NULL);
    	wc.lpszClassName = "MyWindowClass";
    	RegisterClassA(&wc);

    	HWND hWnd = CreateWindowA(/* 参数同前 */);
    	// ...窗口显示代码
}

// 调用
void desktopCommand() {
    CreateDesktopWindow();
}
