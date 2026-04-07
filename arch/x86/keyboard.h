#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

// 键盘中断处理函数
void keyboard_handler(void);

// 扫描码转 ASCII
char scancode_to_ascii(uint8_t scancode);

#endif