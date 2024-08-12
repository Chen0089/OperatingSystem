#include "Include.hh"

int main() {
    // 文件名数组
    const std::string files[] = { "file1.txt", "file2.txt", "file3.txt" };
    size_t count = sizeof(files) / sizeof(files[0]);

    // 遍历检查每个文件是否存在
    for (size_t i = 0; i < count; ++i) {
        if (std::filesystem::exists(files[i])) {
            std::cout << "File exists: " << files[i] << std::endl;
        } else {
            std::cout << "File does not exist: " << files[i] << std::endl;
        }
    }

    return 0;
}
