#include "render.h"
#include "font.h"

static framebuffer_t fb;

void render_init(uint32_t framebuffer_addr) {
    fb.addr = (uint32_t*)framebuffer_addr;
    fb.width = 1024;
    fb.height = 768;
    fb.pitch = fb.width * 4;
    fb.bpp = 32;
}

void put_pixel(int x, int y, color_t color) {
    // 空实现，先保证能编译
    (void)x; (void)y; (void)color;
}

void fill_rect(int x, int y, int w, int h, color_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

void clear_screen(color_t color) {
    (void)color;
}

void render_putchar(char c) {
    (void)c;
}

void render_set_color(color_t fg, color_t bg) {
    (void)fg; (void)bg;
}

void draw_char(int x, int y, char c, color_t fg, color_t bg) {
    (void)x; (void)y; (void)c; (void)fg; (void)bg;
}