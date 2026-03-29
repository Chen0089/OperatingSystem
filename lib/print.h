#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

// 打印字符串
void print(const char* str);

// 带格式化的打印（简化版，仅支持 %s %x %d）
void printf(const char* fmt, ...);

// uint32的打印
void print_uint32(uint32_t num);

#endif