#include "headers/Include.hh"           // 依赖项的include
#include "headers/namespaces.hh"        // 命名空间
#include "headers/define.hh"            // define声明

/*
 * 你可以在这里声明全局变量
 * 但你必须遵循规则
 * 详见codingrule.md
*/
vector<string> bootStartup = 
{
	"test.bat"
};

// 进度条显示函数
void progressBar(int percentage, const string& action) {
    const int barWidth = 50;
    cout << "\r" << action << " [";
    int pos = barWidth * percentage / 100;
    for (int i = 0; i < barWidth; ++i) {
        cout << (i <= pos ? "=" : " ");
    }
    cout << "] " << percentage << "%" << flush;
}

// 下载文件函数（带进度显示）
bool downloadFileWithProgress(const char* url, const char* outputPath) {
    HINTERNET hInternet = InternetOpenA("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hFile = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return false;
    }

    // 获取文件大小
    DWORD fileSize = 0;
    char buf[256];
    DWORD bufLen = sizeof(buf);
    if (!HttpQueryInfoA(hFile, HTTP_QUERY_CONTENT_LENGTH, buf, &bufLen, NULL)) {
        fileSize = 0;
    } else {
        fileSize = atol(buf);
    }

    FILE* fp = fopen(outputPath, "wb");
    if (!fp) {
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[4096];
    DWORD bytesRead;
    DWORD totalRead = 0;
    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) {
        if (bytesRead == 0) break;
        
        fwrite(buffer, 1, bytesRead, fp);
        totalRead += bytesRead;
        
        // 更新进度条
        int percent = fileSize ? (totalRead * 100 / fileSize) : 0;
        progressBar(percent, "下载中");
    }

    progressBar(100, "下载完成");
    cout << endl;

    fclose(fp);
    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);
    return true;
}

// 安装模块命令（从指定分支下载）
void installModule(string name) {
    // 修改为你的GitHub仓库路径
    const char* dllUrl = "https://raw.github.com/chen0089/operatingSystem/Modules/DLL/dll/" + name + "/module.dll";
    const char* dllPath = "module.dll";

    if (downloadFileWithProgress(dllUrl, dllPath)) {
        cout << "模块安装成功！请重启" << endl;
		writeLogFile("安装" + name + "模块");
    } else {
        cout << "下载失败！请检查网络连接" << endl;
		writeLogFile("安装" + name + "模块时发生了错误");
    }
}

// 其余代码保持之前的结构不变...

void changeAllColor(string color) {
	system("color" + color);
	writeLogFile("改变颜色至" + color);
}
void changeColor(int colorNumber = 16) {
	/*
 	 * 设置句柄（handle）:获取到的标准句柄、颜色，颜色值0-16，黑底（某颜色）字
	 * 字符排列：
  	 * 黑 深蓝 深绿 亮蓝 红 紫 金黄 亮灰 灰 蓝 绿 亮蓝 橙 粉 米白 亮白
	 * 1  2    3   4   5  6  7   8   9 10 11 12 13 14  15  16 
	*/
	// 简单判断参数是否正确
	if(colorNumber > 0 && colorNUmber < 17) {continue;}else{cout << "参数不正确" << endl;writeLogFile("改变颜色：参数不正确");}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
}
void changeBackgroundColor(int colorRGB) {
	/*
	 * 第一步也是设置句柄
	 * 直接使用16进制
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
}
int loadModules(string direction) {
	cout << "启动" << direction << "..." << endl;
	
	// 加载DLL
	HMODULE hDll = LoadLibrary(L"DesktopUI.dll");
	if (!hDll) {
	     cout << "无法加载" << direction << endl;
		 writeLogFile("加载" + direction + "时发生了未知的错误");
	     return 1;
	}
	
	// 获取启动函数
	auto start = (
		void(*)
		()
	)
	GetProcAddress(
		hDll,
		"start"
	);
	if (!start) {
	    cout << "找不到" << direction << endl
			 << "请检查是否安装此插件" << endl;
	    FreeLibrary(hDll);
		writeLogFile("加载插件" + direction + "时发生了错误");
	    return 404;
	}
	
	// 启动
	cout << direction << "正在初始化..." << endl;
	writeLogFile("启动" + direcrion);
	start();
	
	// 清理
	FreeLibrary(hDll);
	writeLogFile(direction + "已关闭");
	cout << direction << "已关闭" << endl;
	return 0;
}
// 初始化
void initalzing() {
	// 变量progress，用于表示进度条的进度数据，需要随用随复原（progress = 0;）
	int progress = 14;
	cout << progressBar(progress, "初始化中");
    
	// 变量command，表示用户输入的指令
    string command;
	progress += 14;
	cout << progressBar(progress, "初始化中");
    
	// json结构体jsonString，保存启动项数据
    json jsonString = json(startupItems);
    progress += 14;
	cout << progressBar(progress, "初始化中");

    // 解析startupItems，保存进启动项中，后续将会依次启动启动项
    vector<string> startupItems;
	progress += 14;
	cout << progressBar(progress, "初始化中");
	
    if (
		parseJsonStringArrayToFile(
			"startupItems.json",
			startupItems
		)
	)
	{
        for (const auto& item : startupItems) {
        	run_bat_file(item);
        }
    }
	else {
		writeCrashReportFile(114514191810);// 好臭的错误代码💩
        cout << "失败" << endl;
    }
	progress += 14;
	cout << progressBar(progress, "初始化中");
	
	Directory* root = new Directory("root");
	progress += 14;
	cout << progressBar(progress, "初始化中");

	Directory* current = root;
	progress += 15;
	cout << progressBar(progress, "初始化中");
}

struct Directory {
    string name;
    Directory* parent;
    map<string, Directory*> children;

    Directory(const string& name, Directory* parent = nullptr)
        : name(name), parent(parent) {}

    ~Directory() {
        for (auto& child : children) {
            delete child.second;
        }
    }
};

vector<string> split_line(const string& line) {
    vector<string> args;
    stringstream iss(line);
    string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }
    return args;
}

vector<string> split_path(const string& path) {
    vector<string> parts;
    stringstream ss(path);
    string part;
    while (getline(ss, part, '/')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }
    return parts;
}

Directory* get_root(Directory* dir) {
    while (dir->parent != nullptr) {
        dir = dir->parent;
    }
    return dir;
}

Directory* resolve_path(const string& path, Directory* current_dir) {
    if (path.empty()) return current_dir;

    std::vector<string> parts;
    if (path[0] == '/') {
        current_dir = get_root(current_dir);
        parts = split_path(path.substr(1));
    } else {
        parts = split_path(path);
    }

    for (const auto& part : parts) {
        if (part == ".") {
            continue;
        } else if (part == "..") {
            if (current_dir->parent != nullptr) {
                current_dir = current_dir->parent;
            }
        } else {
            auto it = current_dir->children.find(part);
            if (it == current_dir->children.end()) {
                return nullptr;
            }
            current_dir = it->second;
        }
    }
    return current_dir;
}

string get_full_path(Directory* dir) {
    vector<string> parts;
    while (dir->parent != nullptr) {
        parts.push_back(dir->name);
        dir = dir->parent;
    }
    parts.push_back(dir->name);
    reverse(parts.begin(), parts.end());
    if (parts.size() == 1) return "/";
    ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        oss << "/" << parts[i];
    }
    return oss.str();
}

// 用于存储从站点接口获取的响应数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// 获取最新版本信息
string getLatestReleaseVersion() {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    // 站点版本信息接口链接
    string url = "https://api.github.com/repos/chen0089/OperatingSystem/releases/latest";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // 设置请求头 User-Agent
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 设置回调函数来接收数据
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 执行 HTTP 请求
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cout << "curl_easy_perform失败: " << curl_easy_strerror(res) << endl;
            return "";
        }

        // 使用 nlohmann/json 解析 JSON 响应，获取版本号
        try {
            json root = json::parse(readBuffer);  // 使用 nlohmann::json 解析响应
            string latestVersion = root["tag_name"].get<string>();  // 获取 "tag_name"
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return latestVersion;
        } catch (const json::parse_error& e) {
            cout << "解析JSON数据时失败: " << e.what() << endl;
        }
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return "";
}
bool yn () {
	// 表示用户输入
	string input;
	// 赋值
	cin << input;
	
	while(input != "y" && input != "n") {
		switch (input):
			case "y":
				return true;
				break;
			case "n":
				return false;
				break;
			default:
				cout << "错误！请重新输入！";
				break;
	}
}
void writeCrashReportFile (long long errCode) {
	// 分析错误原因
	string WHY;
	switch (errcode):
		case 0:
			WHY = "正常了嘿嘿"
		case 10:
			WHY = "没有允许写入文本";
		default:
			WHY = "NULL.未定义错误.建议上报github.com/chen0089/operatingSystem/issues";
	ofstream file(crashReport.txt", std::ios::trunc); // 写模式>>清空文件内容
    if (file.is_open()) {
        file << "崩溃报告" << endl << endl << "错误代码：" << errCode << "错误原因：" << WHY; // 将文本写入文件
        file.close(); // 关闭文件
    } else {
        cout << "无法打开文件。建议检查是否允许写入文本" << endl;
    }
}
	
// 参数：（默认系统日志）目录、操作
void writeLogFile (string where = "logFile.log", string controls) {
	tm *ltm = localtime(&now);
	ofstream file(where, std::ios::app); // 以追加模式打开文件
	if (file.is_open()) {
		 // 将文本写入文件
         file 
		 // << endl
		 // 后面说了干什么的

		 // 年份	 
		 << 1900 + ltm->tm_year
	     // 月份 
	     << "/"<< 1 + ltm->tm_mon
   	     // 天
		 << "/" <<  ltm->tm_mday
    	 // 分隔
		 << "|"
		 // 小时
		 << ltm->tm_hour << ":"
   	     // 分
		 << ltm->tm_min << ":"
  	     // 秒
		 << ltm->tm_sec
		 // 分隔
		 << " "
		 // 进行的操作	 
		 << controls
		 //换行，这里是在最后，放到最前面也可以，前面的我注释掉了
		 << endl;
        file.close(); // 关闭文件
    } else {
        cout << "日志错误：无法打开文件\n";
    }
}

// 显示时间
void showTime() {
	// 基于当前系统的当前日期/时间
	time_t now = time(0);
 	cout << "1970 到目前经过秒数:" << now << endl;
	tm *ltm = localtime(&now);
	// 输出 tm 结构的各个组成部分
  	cout << "年: "<< 1900 + ltm->tm_year << endl
	     << "月: "<< 1 + ltm->tm_mon<< endl
   	     << "日: "<<  ltm->tm_mday << endl
    	 << "时间: "<< ltm->tm_hour << ":"
   	     << ltm->tm_min << ":"
  	     << ltm->tm_sec << endl;
	writeLogFile("显示了一次时间");
}

// 显示版本
void showVersion() {
	cout << "   ___" << endl
    	 << "   \\#  \\    ____" << endl
         << "    \\#   \\  /   /" << endl
         << "     \\#   \\/   /" << endl
         << "      \\#___|___/" << endl
         << " \\,OS:v1.1.0" << endl;
}

// CORE: 运行应用
void run_bat_file(const string& bat_file) {
    if (bat_file.substr(bat_file.size() - 4) == ".bat") {
        cout << "正在执行: " << bat_file << endl;

        // 设置启动信息
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        si.cb = sizeof(si);

        // 将 string 转换为 wstring
        int size_needed = MultiByteToWideChar(
	    	CP_UTF8,
	    	0,
	    	bat_file.c_str(),
	    	(int)bat_file.size(),
	    	NULL,
	    	0
		);
        wstring wbat_file(
	    	size_needed,
	    	0
		);
        MultiByteToWideChar(
			CP_UTF8,
			0,
			bat_file.c_str(),
			(int)bat_file.size(),
			&wbat_file[0],
			size_needed
		);

        // 创建进程
        if (
	        CreateProcess(
                NULL,
                const_cast<LPWSTR> (wbat_file.data()),  // BAT 文件路径
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
		        NULL,
                &si,
                &pi
		    )
	    ) {
            // 等待进程结束
            WaitForSingleObject(
			    pi.hProcess,
			    INFINITE
	    	);

            // 获取进程退出码
            DWORD exitCode;
            GetExitCodeProcess(
		    	pi.hProcess,
		    	&exitCode
	    	);
            if (exitCode == 0) {
                cout << "执行成功！" << endl;
				// writeLogFile("打开" + bat_file + " - 完毕");
            }
            else {
                cout << "执行失败，错误代码：" << exitCode << endl;
				writeLogFile("打开" + bat_file + " - 失败");
            }

            // 关闭进程和线程句柄
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
			writeLogFile("打开" + bat_file + " - 完毕");
        }
        else {
            cout << "创建进程失败，错误代码：" << GetLastError() << endl;
			writeLogFile("打开" + bat_file + " - 失败");
        }
    }
    else {
        cout << "执行失败" << endl
	    	 << "命令中没有或只有无效的-.bat文件！你可以输入help获取帮助。" << endl;
		writeLogFile("打开" + bat_file + " - 未找到");// 让系统养成记日记的好习惯
    }
}

// 主循环
void mainWhile() {
	cout << get_full_path(current) << "> ";	// 显示路径信息和命令提示符
    getline(cin, command);					// 也是应该输入了好吧
	if(command.empty()){continue;}			// 检测命令是否为空
    auto args = split_line(line);
    if(args.empty()){continue;}				// 检测arg是否为空

    string command = args[0];

    if (command == "exit") {
        cout << "正在退出命令行系统..." << endl;
    	// 尝试第一次保存
		if (!saveJsonToFile(jsonString, "startupItems.json")) {
    		cout << "似乎无法保存启动项数据！仍要关机？(y/n): ";
			writeLogFile("保存启动项数据发生了错误");
        	if (yn()) {
            	// 如果用户选择关机，直接退出
    	    	cout << "程序正在关闭..." << endl;
    	    	return 1;
			}
			else if (!yn()) {
    	    	// 如果选择重新尝试保存
    	    	cout << "正在重新尝试保存数据..." << endl;
				if (
					!saveJsonToFile(
						jsonString,
						"startupItems.json"
					)
				) {
    	    	    cout << "仍然无法保存启动项数据！关机..." << endl;
					writeLogFile("保存启动项数据发生了错误");
					writeLogFile("关机...");
					writeCrashReportFile(10);
					return 1;
        		}	
        	}
    	}
    	else if (command=="version") {
		    showVersion();
		}
    	else if (command.substr(0, 4) == "help") {
		    // 检查是否有页数参数
			if (command.size() > 5) {
    		    try {
    		    	int page = stoi(command.substr(5)); // 提取页数
    	        	help(page);
				}
        		catch (const invalid_argument& e) {
					cout << "无效的页数参数！请输入数页数。" << endl;
    	    	}
    		}		
			else {
            	cout << "您输入的参数过短！请输入页数。例如：help 1" << endl;
       		}	
    	}
		else if(command="clear") {
			system(cls);
    	}
		else if (command == "cd") {
		    if (args.size() < 2) {
				cout << "用法: cd <path>" << endl;
    		        continue;
    	    }
    	    Directory* target = resolve_path(args[1], current);
		    if (target) {
				current = target;
    		}
			else {
    	        cout << "路径未找到: " << args[1] << endl;
		    }
		}
		else if (command == "dir") {
			if (args.size() == 1) {  // 列出目录
				for (const auto& pair : current->children) {
    	    	    cout << pair.first << " ";
    	    	}
	        	cout << endl;
	    	}
			else {  // 创建目录
    			for (
					size_t i = 1;
					i < args.size();
					++i
				)
				{
    	    	    string dirname = args[i];
    	    	    if (current->children.find(dirname) != current->children.end()) {
    			    	cout << "Directory already exists: " << dirname << endl;
    			    }
					else {
       	    	    	current->children[dirname] = new Directory(dirname, current);
					}
    	    	}
    		}	
			else if (command == "ls") {
    	    	for (const auto& pair : current->children) {
    	    	    cout << pair.first << " ";
    	    	}
    	    	cout << endl;
    		}
    		else if(command = "") {
    		    cout << "命令无效！命令不能为空!" << endl;
			}
    		else {
    		    run_bat_file(command);  // 执行 .bat 文件
    		}
		}
	}

// 帮助信息
void help(int page) {
    switch(page):
		case 1:
			cout << "help: 显示帮助" << endl
			     << "exit: 退出命令行系统" << endl
	    	     << "version: 显示版本号" << endl
			     << "clear: 清屏" << endl
				 << "cd: 切换目录" << endl
				 << "dir: 列出目录" << endl
				 << "ls: 列出目录" << endl;
			break;
		default:
        	cout << "您输入的页数不正确，开发者编撰的页面是1-1" << endl;
}

// 将 JSON 保存到文件
bool saveJsonToFile(const json& j, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << j.dump( 4 ) << endl;  // 美化 JSON 格式输出
        outFile.close();
        return true;
    }
    return false;
}

// 更新提示
void isLatest() {
	string latestVersion = getLatestReleaseVersion();
    if (latestVersion != string(CORRENT_VERSION)) {
		cout << "居然有新版本可以更新!" << endl;
		continue;
	}
    else if (latestVersion == string(CORRENT_VERSION)) {
		continue;
	}
    else {
		cout << "检测更新发生了未知的错误" << endl;
		writeLogFile("检测更新函数发生了未知的错误");
		continue;
	}
}

// 主函数
int main() {
    showVersion();
    cout << "此版本最后更新：2025/03/18,正在检测更新…" << endl;

    isLatest();

	initzaling();

	Sleep(1000);
	
	// 简简单单清个屏，再显示版本
	system(cls);
	showVersion();

	// 系统主循环
    while (1) {
        mainWhile();
	}
		
	delete root;
	writeCrashReportFile(0);// 清空错误报告
	return 0;// 别忘了要返回值！！！
}
