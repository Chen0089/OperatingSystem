#include "fs.h"
#include "../lib/string.h"      // 需要实现或包含 strncpy、strcmp、memcpy
#include "../lib/print.h"       // 打印函数

static fs_t fs;

void fs_init(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        fs.files[i].in_use = 0;
        fs.files[i].size = 0;
        fs.files[i].name[0] = '\0';
    }
}

int fs_create(const char* name) {
    if (!name || name[0] == '\0') return -1;

    for (int i = 0; i < MAX_FILES; i++) {
        if (!fs.files[i].in_use) {
            strncpy(fs.files[i].name, name, MAX_FILENAME - 1);
            fs.files[i].name[MAX_FILENAME - 1] = '\0';
            fs.files[i].size = 0;
            fs.files[i].in_use = 1;
            return 0;   // 成功
        }
    }
    return -1;   // 文件表满
}

int fs_write(const char* name, const uint8_t* buffer, uint32_t size) {
    if (!name || !buffer || size == 0 || size > MAX_FILE_SIZE) return -1;

    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use && strcmp(fs.files[i].name, name) == 0) {
            memcpy(fs.files[i].data, buffer, size);
            fs.files[i].size = size;
            return size;   // 返回写入字节数
        }
    }
    return -1;   // 文件不存在
}

int fs_read(const char* name, uint8_t* buffer, uint32_t size) {
    if (!name || !buffer || size == 0) return -1;

    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use && strcmp(fs.files[i].name, name) == 0) {
            uint32_t read_size = (size < fs.files[i].size) ? size : fs.files[i].size;
            memcpy(buffer, fs.files[i].data, read_size);
            return read_size;   // 返回实际读取字节数
        }
    }
    return -1;   // 文件不存在
}

int fs_delete(const char* name) {
    if (!name) return -1;

    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use && strcmp(fs.files[i].name, name) == 0) {
            fs.files[i].in_use = 0;
            fs.files[i].size = 0;
            fs.files[i].name[0] = '\0';
            return 0;   // 成功
        }
    }
    return -1;   // 文件不存在
}

void fs_list(void) {
    int count = 0;
    print("Files:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (fs.files[i].in_use) {
            print("  ");
            print(fs.files[i].name);
            print("  ");
            print_uint32(fs.files[i].size);
            print(" bytes\n");
            count++;
        }
    }
    if (count == 0) {
        print("  (empty)\n");
    }
}