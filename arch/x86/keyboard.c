#include "idt.h"
#include "print.h"

void keyboard_init(void) {
    // 启用键盘中断（IRQ1）
    outb(0x21, inb(0x21) & 0xFD);  // 设置 PIC 掩码
}
static void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);  // 读取扫描码
    
    if (scancode & 0x80) {
        // 按键释放
        // print("Key released\n");
    } else {
        // 按键按下
        print("Key pressed: ");
        print_hex(scancode);
        print("\n");
    }
    
    // 发送 EOI 给 PIC
    outb(0x20, 0x20);
}
char scancode_to_ascii(uint8_t scancode) {
    static char table[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
    };
    if (scancode < sizeof(table)) {
        return table[scancode];
    }
    return 0;
}
static void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
    
    if (!(scancode & 0x80)) {
        char ascii = scancode_to_ascii(scancode);
        if (ascii) {
            print("You pressed: ");
            print_char(ascii);
            print("\n");
        }
    }
    
    outb(0x20, 0x20);
}