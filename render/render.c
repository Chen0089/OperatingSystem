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
    if (x < 0 || x >= fb.width || y < 0 || y >= fb.height) return;
    fb.addr[y * fb.width + x] = color;
}

void fill_rect(int x, int y, int w, int h, color_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

void clear_screen(color_t color) {
    (void)color;
}

void render_putchar(char c) {
    int max_x = fb.width / 8;
    int max_y = fb.height / 16;

    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        draw_char(cursor_x, cursor_y, c, text_color, bg_color);
        cursor_x++;
        if (cursor_x >= max_x) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= kmax_y) {
        // 清屏
        fill_rect(0, 0, fb.width, fb.height, bg_color);
        cursor_x = 0;
        cursor_y = 0;
    }
}

void render_set_color(color_t fg, color_t bg) {
    (void)fg; (void)bg;
}

void draw_char(int x, int y, char c, color_t fg, color_t bg) {
    unsigned char* glyph = font[(unsigned char)c];
    for (int row = 0; row < 16; row++) {
        unsigned char row_bits = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (row_bits & (1 << (7 - col))) {
                put_pixel(x * 8 + col, y * 16 + row, fg);
            } else {
                put_pixel(x * 8 + col, y * 16 + row, bg);
            }
        }
    }
}

int render_get_width(void) {
    return fb.width;
}

int render_get_height(void) {
    return fb.height;
}