#include "Include.hh"           // 依赖项的include
#include "globalVariable.hh"    // 声明局部变量
#include "namespaces.hh"        // 命名空间

void run_bat_file(const string& bat_file) {
    if (bat_file.substr(bat_file.size() - 4) == ".bat") {
        cout << "正在执行: " << bat_file << endl;

        // 设置启动信息
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        si.cb = sizeof(si);

        // 将 std::string 转换为 std::wstring
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
    cout << "最后更新：2025/01/09,如要更新，请前往chen0089.github.io/OperatingSystem" << endl
         << "初始化中，请耐心等待..." << endl;

    //初始化
    string command;

    cout << "初始化成功，命令行系统已启动。输入 “exit” 来退出，输入 “help” 获取帮助。" << endl;

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command == "exit") {
            cout << "退出命令行系统" << endl;
            break;
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
        else if(command = "") {
            cout << "命令无效！命令不能为空!" << endl;
	}
        else {
            run_bat_file(command);  // 执行 .bat 文件
        }
    }
    return 0;
}
