#include "Include.hh"

int main() {
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

    return 0;
}
