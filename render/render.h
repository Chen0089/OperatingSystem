#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

typedef uint32_t color_t;

// 帧缓冲
typedef struct {
    uint32_t* addr;
    int width;
    int height;
    int pitch;
    int bpp;
} framebuffer_t;

// 图形基础
void render_init(uint32_t framebuffer_addr);
void put_pixel(int x, int y, color_t color);
void fill_rect(int x, int y, int w, int h, color_t color);
void clear_screen(color_t color);

int render_get_width(void);
int render_get_heights(void);
// 字符渲染
void render_putchar(char c);
void render_set_color(color_t fg, color_t bg);
void draw_char(int x, int y, char c, color_t fg, color_t bg);

// 颜色宏
#define RGB(r,g,b)  ((0xFF << 24) | ((r) << 16) | ((g) << 8) | (b))
#define COLOR_BLACK     RGB(0,0,0)
#define COLOR_WHITE     RGB(255,255,255)
#define COLOR_RED       RGB(255,0,0)
#define COLOR_GREEN     RGB(0,255,0)
#define COLOR_BLUE      RGB(0,0,255)

#endif