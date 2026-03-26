// print.c
#include "print.h"
#include "render.h"
#include <stdarg.h>

void print(const char* str) {
    while (*str) {
        render_putchar(*str++);
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's':
                    print(va_arg(args, const char*));
                    break;
                case 'd':
                    // 打印十进制（需要实现）
                    break;
                case 'x':
                    // 打印十六进制
                    break;
                default:
                    render_putchar('%');
                    render_putchar(*fmt);
                    break;
            }
        } else {
            render_putchar(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}