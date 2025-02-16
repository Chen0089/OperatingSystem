/*
		 else {
            cout << "Unknown command: " << command << endl;
        }
    }
}
*/
#include "Include.hh"           // 依赖项的include
#include "namespaces.hh"        // 命名空间
#include "define.hh"            // define声明

/*
 * 你可以在这里声明全局变量
 * 但你必须遵循规则
 * 详见codingrule.md
*/
vector<string> bootStartup = 
{
	"test.bat"
};
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

// 用于存储从 GitHub API 获取的响应数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string ProgressBar (short progress) {
	if (progress < 6) {
		return "";
	}
	else if (progress > 5 && progress < 16) {
		return "="		
	}
	else if (progress > 15 && progress < 26) {
		return "==";
	}
	else if (progress > 25 && progress < 36) {
		return "===";
	}
	else if (progress > 35 && progress < 46) {
		return "====";
	}
	else if (progress > 45 && progress < 56) {
		return "=====";
	}
	else if (progress > 55 && progress < 66) {
		return "======";
	}
	else if (progress > 55 && progress < 66) {
		return "=======";
	}
	else if (progress > 65 && progress < 77) {
		return "========";
	}
	else if (progress > 76 && progress < 91) {
		return "=========";
	}
	else if (progress > 90 && progress < 100) {
		return "==========";
	}
	else {
		return "err to print";
	}
}
// 获取 GitHub 最新版本信息
string getLatestReleaseVersion() {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    // GitHub Release API URL
    string url = "https://api.github.com/repos/" + 
	string(GITHUB_OWNER) + 
	"/" + 
	string(GITHUB_REPO) +
	"/releases/latest";

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
        cout << "无法打开文件。建议检查一下您是否允许写入文本吗？\n";
    }
}
void writeLogFile (string where, string controls) {
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
}
void showVersion() {
	cout << "   ___" << endl
    	 << "   \\#  \\    ____" << endl
         << "    \\#   \\  /   /" << endl
         << "     \\#   \\/   /" << endl
         << "      \\#___|___/" << endl
         << " \\,OS:v1.1.0" << endl;
}
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
            }
            else {
                cout << "执行失败，错误代码：" << exitCode << endl;
            }

            // 关闭进程和线程句柄
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            cout << "创建进程失败，错误代码：" << GetLastError() << endl;
        }
    }
    else {
        cout << "执行失败" << endl
	    	 << "命令中没有或只有无效的-.bat文件！你可以输入help获取帮助。" << endl;
    }
}
void help(int page) {
    if (page == 1) {
		cout << "help: 显示帮助" << endl
		     << "exit: 退出命令行系统" << endl
	         << "version: 显示版本号" << endl
		     << "clear: 清屏" << endl;
	}
	else {
        cout << "您输入的页数不正确，开发者编撰的页面是1-1" << endl;
	}
}
// 将 vector 转换为 JSON 的函数
json vectorToJson(const vector<string>& startupItems) {
    return json(startupItems);  // 直接将 vector 转换为 JSON 数组
}

// 将 JSON 保存到文件的函数
bool saveJsonToFile(const json& j, const string& filename) {
    ofstream outFile(filename);
    if ( outFile.is_open() ) {
        outFile << j.dump( 4 ) << endl;  // 美化 JSON 格式输出
        outFile.close();
        return true;
    }
    return false;
}

int main() {
    showVersion();
    cout << "最后更新：2025/02/07,正在检测更新…" << endl;

    string latestVersion = getLatestReleaseVersion();
    if (latestVersion != "1.1.0") {
		cout << "哇噻！居然有新版本！快去github更新!（也有可能是因为你是测试版哦～）" << endl;
	}
    else if (latestVersion == "1.1.0") {
		break;
	}
    else {
		cout << "发生未知错误" << endl;
		writeCrashReportFile(520);// 唉...这个彩蛋可能用户永远也无法发现...
		return 1;
	}

    //初始化
	// 变量progress，用于表示进度条的进度数据，需要随用随复原（progress = 0;）
	int progress = 16;
    // 变量command，表示用户输入的指令
    string command;
	progress += 16;
	cout << "System Initalzing:[" << progressBar(progress) << "]"
		 << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    // json结构体jsonString，保存启动项数据
    json jsonString = vectorToJson(startupItems);
    progress += 16;

    // 解析startupItems，保存进启动项中，后续将会依次启动启动项
    vector<string> startupItems;
	
    if (
		parseJsonStringArrayToFile(
			"startupItems.json",
			startupItems
		)
	) {
        for (const auto& item : startupItems) {
            // 这里由输出改运行，后续改成函数，运行item
        }
    } else {
		writeCrashReportFile(114514191810);// 好臭的错误代码💩
        cout << "失败" << endl;
    }
	progress += 16;
	cout << "System Initalzing:[" << progressBar(progress) << "]"
		 << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	
	Directory* root = new Directory("root");
	progress += 16;
	cout << "System Initalzing:[" << progressBar(progress) << "]"
		 << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
    
	Directory* current = root;
	progress += 17;
	cout << "System Initalzing:[" << progressBar(progress) << "]"
		 << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	// 简简单单清个屏，再显示版本
	system(cls);
	showVersion();

    cout << "初始化成功，命令行系统已启动。正在运行启动项" << endl;
	// 使用传统的for循环
    for (int i = 0; i < startupItems.size(); ++i) {
        run_bat_file(startupItems[i]);
    }
    
    /*
	或者使用范围for（range-based for）循环
    
	for (int value : vec) {
        process(value);
    }
	*/
	cout << "启动项运行完毕。输入 “help” 获取帮助。" << endl;

	// 系统主循环
    while (1) {
        cout << get_full_path(current) << "> ";
        getline(cin, command);

		if (command.empty()) continue;

        auto args = split_line(line);
        if (args.empty()) continue;

        string command = args[0];


    if (command == "exit") {
        cout << "正在退出命令行系统..." << endl;
        // 尝试第一次保存
    if (!saveJsonToFile(jsonString, "startupItems.json")) {
        cout << "似乎无法保存启动项数据！仍要关机？(y/n): ";
        
        if (yn()) {
            // 如果用户选择关机，直接退出
            cout << "程序正在关闭..." << endl;
            return 1;
        } else if (!yn()) {
            // 如果选择重新尝试保存
            cout << "正在重新尝试保存数据..." << endl;
            if (
				!saveJsonToFile(
					jsonString,
					"startupItems.json"
				)
			) {
                cout << "仍然无法保存启动项数据！关机..." << endl;
				writeCrashReportFile(10);
                return 1;
            }
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
        	cout << "Usage: cd <path>" << endl;
                continue;
        }
        Directory* target = resolve_path(args[1], current);
        if (target) {
            current = target;
        }
		else {
            cout << "Path not found: " << args[1] << endl;
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
    	for (size_t i = 1; i < args.size(); ++i) {
            string dirname = args[i];
            if (current->children.find(dirname) != current->children.end()) {
            	cout << "Directory already exists: " << dirname << endl;
            }
			else {
                    current->children[dirname] = new Directory(dirname, current);
			}
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

	delete root;
	writeCrashReportFile(0);
	return 0;// 别忘了要返回值！！！
}
