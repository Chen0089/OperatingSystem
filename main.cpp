#include "Include.hh"

void clearScreen() {
    // Windows specific: clears the console screen
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    COORD coord = {0, 0};

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hStdOut, ' ', dwConSize, coord, NULL);
    SetConsoleCursorPosition(hStdOut, coord);
}

void hideConsoleWindow() {
    // Windows specific: hides the console window
    HWND hWnd = GetConsoleWindow();
    if (hWnd != NULL) {
        ShowWindow(hWnd, SW_HIDE);
    }
}

// 全局变量
CComPtr<IWebBrowser2> webBrowser; // WebBrowser 控件的指针

// 前向声明
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitializeBrowser(HWND hwnd);

void InitializeBrowser(HWND hwnd) {
    // 初始化 COM 库
    CoInitialize(nullptr);

    // 创建 WebBrowser 控件实例
    CComPtr<IWebBrowser2> browser;
    CComPtr<IDispatch> dispatch;
    browser.CoCreateInstance(CLSID_WebBrowser, nullptr, CLSCTX_INPROC_SERVER);

    // 设置 WebBrowser 控件的位置和大小
    RECT rect;
    GetClientRect(hwnd, &rect);
    browser->put_Left(0);
    browser->put_Top(0);
    browser->put_Width(rect.right - rect.left);
    browser->put_Height(rect.bottom - rect.top);
    browser->put_Visible(VARIANT_TRUE);

    // 导航到本地 HTML 文件
    wchar_t filePath[MAX_PATH];
    GetModuleFileName(nullptr, filePath, MAX_PATH); // 获取可执行文件的路径
    PathRemoveFileSpec(filePath); // 从路径中移除可执行文件名
    PathAppend(filePath, L"index.html"); // 附加 HTML 文件名

    VARIANT url;
    VariantInit(&url);
    url.vt = VT_BSTR;
    url.bstrVal = SysAllocString(filePath); // 设置 URL 为本地文件
    browser->Navigate2(&url, nullptr, nullptr, nullptr, nullptr); // 导航到该文件
    VariantClear(&url);

    // 将 WebBrowser 控件与窗口关联
    browser->QueryInterface(IID_IDispatch, (void**)&dispatch);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dispatch.p);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE: {
            // 窗口大小调整时调整 WebBrowser 控件的大小
            if (webBrowser) {
                RECT rcClient;
                GetClientRect(hwnd, &rcClient);
                webBrowser->put_Width(rcClient.right - rcClient.left);
                webBrowser->put_Height(rcClient.bottom - rcClient.top);
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0); // 发送退出消息以退出应用程序
            CoUninitialize(); // 清理 COM 库
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // 默认窗口过程
    }
    return 0;
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 文件名数组
    const string files[] = {
        "main.cpp",
        "Include.hh"
    };
    size_t count = sizeof(files) / sizeof(files[0]);

    // 遍历检查每个文件是否存在
    for (size_t i = 0; i < count; ++i) {
        if (filesystem::exists(files[i])) {
            cout << "File finded: " << files[i] << endl;
        }
        else {
            cout << "ERROR:File does not finded: " << files[i] << endl << ">>>CRASH!<<<" << endl << "code:1";
            return 1;
        }
    }

    system("cls");
    hideConsoleWindow();
    const char CLASS_NAME[] = "HTML Viewer";

    // 注册窗口类
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "HTML Viewer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, nullptr);

    if (hwnd == nullptr) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    InitializeBrowser(hwnd); // 初始化 WebBrowser 控件

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
