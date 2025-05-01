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

bool yn () {
	// 表示用户输入
	string input;
	// 赋值
	std::cin >> input;
	
	while(input != "y" && input != "n") {
		if(input=="y") {
			return true;
			break;
		}
		if (input=="n") {
			return false;
			break;
		}
		else {
			cout << "错误！请重新输入！";
			break;
		}
	}
}
void writeCrashReportFile (long long errCode) {
	// 分析错误原因
	string WHY;

	switch (errCode) {
    	case 0:
        	WHY = "正常了嘿嘿";
	        break;
    	case 10:
        	WHY = "没有允许写入文本";
        	break;
    	default:
        	WHY = "NULL.未定义错误.建议上报github.com/chen0089/operatingSystem/issues";
        	break;
	}

	ofstream file("crashReport.txt", std::ios::trunc); // 写模式>>清空文件内容
    if (file.is_open()) {
        file << "崩溃报告" << endl << endl << "错误代码：" << errCode << "错误原因：" << WHY; // 将文本写入文件
        file.close(); // 关闭文件
    } else {
        cout << "无法打开文件。建议检查是否允许写入文本" << endl;
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
}

// 显示版本
void showVersion() {
	cout << "   ___" << endl
    	 << "   \\#  \\    ____" << endl
         << "    \\#   \\  /   /" << endl
         << "     \\#   \\/   /" << endl
         << "      \\#___|___/" << endl
         << " \\,OS:v1.0(NOBOOT)" << endl;
}

// 帮助信息
void help(int page) {
    switch(page){
		case 1:
			cout << "help: 显示帮助" << endl
			     << "exit: 退出命令行系统" << endl
	    	     << "version: 显示版本号" << endl
			     << "clear: 清屏" << endl
				 << "cd: 切换目录" << endl
				 << "dir: 列出目录" << endl
				 << "ls: 列出目录" << endl;
		default:
        	cout << "您输入的页数不正确，开发者编撰的页面是1-1" << endl;
	}
}

// 将 JSON 保存到文件
bool saveJsonToFile(
	const json& j,
	const string& filename
	) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << j.dump( 4 ) << endl;  // 美化 JSON 格式输出
        outFile.close();
        return true;
    }
    return false;
}

// 主循环
void mainWhile() {
while (true) {
	string command; // 命令
    if (command == "exit") {
        cout << "正在退出命令行系统..." << endl;
			break; // 退出循环
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
    	else if(command == "") {
    		cout << "命令无效！命令不能为空!" << endl;
		}
	}
}

// 主函数
int main() {
    showVersion();
    cout << "此版本最后更新：2025/05/01" << endl;
	
	// 显示版本

	showVersion();

	// 系统主循环
    mainWhile();

	writeCrashReportFile(0);// 清空错误报告
	return 0;// 别忘了要返回值！！！
}
