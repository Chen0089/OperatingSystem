#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

// 基础字符输出（需由你实现，写显存或串口）
void putchar(char c);

// 打印字符串
void print(const char* str);

// 打印十六进制数
void print_hex(uint32_t num);

// 打印十进制数
void print_dec(uint32_t num);

// 打印整数（自动判断格式，简单实现）
void print_int(int32_t num);

// 带格式化的打印（简化版，仅支持 %s %x %d）
void printf(const char* fmt, ...);

#endif