#include "arch/x86/idt.h"
#include "lib/print.h"
#include "render/render.h"
#include "fs/fs.h"

// 内核入口（接收显存地址）
void kernel_main(uint32_t framebuffer_addr) {
    // 初始化图形渲染（传显存地址）
    render_init(framebuffer_addr);
    
    // 清屏（黑色）
    clear_screen(COLOR_BLACK);
    
    // 设置文本颜色（白字黑底）
    render_set_color(COLOR_WHITE, COLOR_BLACK);
    
    // 打印信息（用新的 print 函数）
    print("Hello from MyOS kernel v0.1.2-dev!\n");
    print("This is a 32-bit protected mode kernel.\n");
    print("Graphics mode initialized.\n");
    print("Next step: rendering.\n");
    
    // 初始化 IDT
    idt_init();
    asm volatile("sti");
    
    // 画点测试（画一个红点）
    put_pixel(512, 384, COLOR_RED);
    
    // 画一条白线（水平线）
    for (int x = 0; x < render_get_width(); x++) {
        put_pixel(x, 384, COLOR_WHITE);
    }
    
    // 死循环
    while(1);
}