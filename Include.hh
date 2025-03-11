// 谁还没有个加速哇
#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <shlwapi.h>
#include <netlistmgr.h>  // 网络状态检测
#include <wrl/client.h>  // ComPtr
#include <string>
#include <chrono>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")

using namespace Gdiplus;
using namespace Microsoft::WRL;
