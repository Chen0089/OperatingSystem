#include "Include.hh"           // 依赖项的include
#include "namespaces.hh"        // 命名空间
#include "define.hh"            // define声明

/*
 * 你可以在这里声明全局变量
 * 但你必须遵循规则
 * 详见codingrule.md
*/
vector<string> bootStartup;

// 用于存储从 GitHub API 获取的响应数据
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// 获取 GitHub 最新版本信息
string getLatestReleaseVersion() {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    // GitHub Release API URL
    string url = "https://api.github.com/repos/" + string(GITHUB_OWNER) + "/" + string(GITHUB_REPO) + "/releases/latest";

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
            cout << "curl_easy_perform失败: " << curl_easy_strerror(res) << std::endl;
            return "";
        }

        // 解析 JSON 响应，获取版本号
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(readBuffer, root)) {
            string latestVersion = root["tag_name"].asString();
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return latestVersion;
        } else {
            cout << "解析JSON数据时失败" << endl;
        }
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return "";
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
	cout << "   ___                    " << endl
    	 << "   \\#  \\    ____        " << endl
         << "    \\#   \\  /   /       " << endl
         << "     \\#   \\/   /        " << endl
         << "      \\#___|___/         " << endl
         << " \\,OS:v1.1.0             " << endl;
}
void run_bat_file(const string& bat_file) {
    if (bat_file.substr(bat_file.size() - 4) == ".bat") {
        cout << "正在执行: " << bat_file << endl;

        // 设置启动信息
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        si.cb = sizeof(si);

        // 将 string 转换为 wstring
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, bat_file.c_str(), (int)bat_file.size(), NULL, 0);
        wstring wbat_file(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, bat_file.c_str(), (int)bat_file.size(), &wbat_file[0], size_needed);

        // 创建进程
        if (CreateProcess(
            NULL,
            const_cast<LPWSTR>(wbat_file.data()),  // BAT 文件路径
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
        {
            // 等待进程结束
            WaitForSingleObject(pi.hProcess, INFINITE);

            // 获取进程退出码
            DWORD exitCode;
            GetExitCodeProcess(pi.hProcess, &exitCode);
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
		     << "exit: 退出命令行系统" << endl;
	}
	else {
        cout << "您输入的页数不正确，开发者编撰的页面是1-1" << endl;
	}
}

int main() {
    showVersion();
    cout << "最后更新：2025/01/20,如要更新，请前往chen0089.github.io/OperatingSystem" << endl
         << "初始化中，请耐心等待..." << endl;

    //初始化
    cout << "variable->command•••";
    string command;
    cout << "OK" << endl;

    cout << "variable->yn•••";
    string yn;
    cout << "OK" << endl;

    cout << "PARSE->startupItems•••" << endl;
    vector<string> startupItems;
    if (parseJsonStringArrayToFile("startupItems.json", startupItems)) {
        for (const auto& item : startupItems) {
            cout << "OK" << endl
				 << item << endl;
        }
    } else {
        cout << "失败" << endl;
    }
	
    cout << "初始化成功，命令行系统已启动。输入 “help” 获取帮助。" << endl;

    while (true) {
        cout << "> ";
        getline(cin, command);

    if (command == "exit") {
        cout << "正在退出命令行系统..." << endl;
        string jsonString = vectorToJson(startupItems);
        // 将JSON字符串存储到文件中
        ofstream outFile("startupItems.json");
        if (outFile.is_open()) {
            outFile << jsonString << endl;
            outFile.close();
		}
	    else {
            cout << "似乎无法保存启动项数据！仍要关机？(y/n)";
	        cin >> yn;
			if (yn=="y") {break;}
            else if (yn=="n") {
		    // 再次将JSON字符串存储到文件中
            	ofstream outFile("startupItems.json");
                if (outFile.is_open()) {
                	outFile << jsonString << endl;
                	outFile.close(); 
                }
	            else {
                    cout << "仍然无法保存启动项数据！关机...";
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
    else if(command = "") {
        cout << "命令无效！命令不能为空!" << endl;
	}
    else {
        run_bat_file(command);  // 执行 .bat 文件
    }
    
    return 0;// 别忘了要返回值！！！
}
