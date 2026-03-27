#include "fs/fs.h"

static fs_t fs;

void fs_init(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        fs.files[i].in_use = 0;
        fs.files[i].size = 0;
        fs.files[i].name[0] = '\0';
    }
}

int fs_create(const char* name) {
    (void)name;
    return -1;
}

int fs_write(const char* name, const uint8_t* buffer, uint32_t size) {
    (void)name;
    (void)buffer;
    (void)size;
    return -1;
}

int fs_read(const char* name, uint8_t* buffer, uint32_t size) {
    (void)name;
    (void)buffer;
    (void)size;
    return -1;
}

int fs_delete(const char* name) {
    (void)name;
    return -1;
}

void fs_list(void) {
    // 空实现
}