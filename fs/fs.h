#ifndef FS_H
#define FS_H

#include <stdint.h>

#define MAX_FILES     32
#define MAX_FILENAME  28
#define MAX_FILE_SIZE 4096

// 文件描述符
typedef struct {
    char name[MAX_FILENAME];
    uint32_t size;
    uint8_t data[MAX_FILE_SIZE];
    uint8_t in_use;
} file_desc_t;

// 文件系统
typedef struct {
    file_desc_t files[MAX_FILES];
} fs_t;

// 初始化
void fs_init(void);

// 创建文件
int fs_create(const char* name);

// 写入文件
int fs_write(const char* name, const uint8_t* buffer, uint32_t size);

// 读取文件
int fs_read(const char* name, uint8_t* buffer, uint32_t size);

// 删除文件
int fs_delete(const char* name);

// 列出所有文件
void fs_list(void);

#endif