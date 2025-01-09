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
// 列出当前目录下的所有文件
void list_files_in_directory(const string& directory) {
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            cout << entry.path().filename() << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "错误: 无法访问目录。" << endl;
    }
}

// 创建一个新文件
void create_file(const string& filename) {
    try {
        ofstream file(filename);
        if (file) {
            file << "这是一个新创建的文件！\n";
            file.close();
            cout << "文件 " << filename << " 已成功创建。" << endl;
        } else {
            cerr << "无法创建文件：" << filename << endl;
        }
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
    }
}

// 删除一个文件
void delete_file(const string& filename) {
    try {
        if (fs::remove(filename)) {
            cout << "文件 " << filename << " 已删除。" << endl;
        } else {
            cerr << "无法删除文件，文件不存在或无法访问。" << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "错误: 无法删除文件。" << endl;
    }
}


int main() {
    cout << "最后更新：2025/01/09,如要更新，请前往chen0089.github.io/OperatingSystem" << endl
         << "初始化中，请耐心等待..." << endl;

    //初始化
    string command;

    cout << "初始化成功，命令行系统已启动。输入 'exit' 来退出。" << endl;
    
    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command == "exit") {
            cout << "退出命令行系统" << endl;
            break;
        } 
        else if (command == "list") {
            // 列出当前目录下的文件
            list_files_in_directory(".");
        }
        else if (command.find("createfile ") == 0) {
            // 创建文件
            string filename = command.substr(11);  // 获取文件名
            create_file(filename);
        }
        else if (command.find("deletefile ") == 0) {
            // 删除文件
            string filename = command.substr(11);  // 获取文件名
            delete_file(filename);
        }
        else {
            run_bat_file(command);  // 执行 .bat 文件
        }
    }
    return 0;
}
