// kernel.c - 内核入口
// 版本: v0.0.1-dev

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// 直接操作显存（文本模式 0xB8000）
#define VIDEO_MEMORY ((uint16_t*)0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// 颜色定义
#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GREY 0x7
#define COLOR_DARK_GREY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

// 清屏
void clear_screen() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VIDEO_MEMORY[i] = (uint16_t)' ' | (COLOR_WHITE << 8);
    }
}

// 打印字符串（带颜色）
void print(const char* str, uint8_t color) {
    static int row = 0, col = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            row++;
            col = 0;
            continue;
        }
        
        if (col >= SCREEN_WIDTH) {
            col = 0;
            row++;
        }
        
        if (row >= SCREEN_HEIGHT) {
            // 屏幕满了，简单处理：清屏并回到顶部
            clear_screen();
            row = 0;
            col = 0;
        }
        
        VIDEO_MEMORY[row * SCREEN_WIDTH + col] = (uint16_t)str[i] | (color << 8);
        col++;
    }
}

// 内核入口
void kernel_main() {
    clear_screen();
    print("Hello from MyOS kernel v0.0.1-dev!\n", COLOR_GREEN);
    print("This is a 32-bit protected mode kernel.\n", COLOR_CYAN);
    print("Next step: IDT and interrupts.\n", COLOR_YELLOW);
    
    // 死循环
    while(1);
}