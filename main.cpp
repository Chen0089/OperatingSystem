#include "Include.hh"

void run_bat_file(const string& bat_file) {
    if (bat_file.substr(bat_file.size() - 4) == ".bat") {
        cout << "正在执行: " << bat_file << endl;

        // 设置启动信息
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        
        si.cb = sizeof(si);
        
        // 创建进程
        if (CreateProcess(
                NULL, 
                const_cast<char*>(bat_file.c_str()),  // BAT 文件路径
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
            } else {
                cout << "执行失败，错误代码：" << exitCode << endl;
            }

            // 关闭进程和线程句柄
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        } else {
            cerr << "创建进程失败，错误代码：" << GetLastError() << endl;
        }
    } else {
        cout << "无效的文件格式！请提供一个 .bat 文件。" << endl;
    }
}

int main() {
    cout << "最后更新：2025/01/09,如要更新，请前往chen0089.github.io/OperatingSystem" << endl
         << "初始化中，请耐心等待..." << endl;

    //初始化
    string command;

    cout << "初始化成功，命令行系统已启动。输入 'exit' 来退出。" << endl;
    
    while (true) {
        cout << "请输入命令: ";
        getline(cin, command);

        if (command == "exit") {
            cout << "退出命令行系统" << endl;
            break;
        } else {
            run_bat_file(command);
        }
    }

    return 0;
}
