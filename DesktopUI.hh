#pragma once

#ifdef DESKTOPUI_EXPORTS
#define DESKTOPUI_API __declspec(dllexport)
#else
#define DESKTOPUI_API __declspec(dllimport)
#endif

// 初始化桌面系统
extern "C" DESKTOPUI_API void InitializeDesktop();

// 创建主窗口
extern "C" DESKTOPUI_API void CreateMainWindow();

// 处理消息循环
extern "C" DESKTOPUI_API void RunMessageLoop();

// 清理资源
extern "C" DESKTOPUI_API void ShutdownDesktop();
