#ifndef STRING_H
#define STRING_H

#include <stddef.h>   // size_t

// 计算字符串长度
size_t strlen(const char* str);

// 字符串拷贝
char* strcpy(char* dest, const char* src);

// 带长度限制的字符串拷贝
char* strncpy(char* dest, const char* src, size_t n);

// 字符串连接
char* strcat(char* dest, const char* src);

// 字符串比较
int strcmp(const char* s1, const char* s2);

// 带长度限制的字符串比较
int strncmp(const char* s1, const char* s2, size_t n);

// 内存拷贝
void* memcpy(void* dest, const void* src, size_t n);

// 内存移动（处理重叠区域）
void* memmove(void* dest, const void* src, size_t n);

// 内存设置（填充）
void* memset(void* s, int c, size_t n);

// 内存比较
int memcmp(const void* s1, const void* s2, size_t n);

// 查找字符在字符串中的位置
char* strchr(const char* s, int c);

// 查找字符在字符串中最后一次出现的位置
char* strrchr(const char* s, int c);

#endif