#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

void putpixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b);

void init_wm(char *walpaperData);
void draw_filled_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t r, uint8_t g, uint8_t b);
void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t r, uint8_t g, uint8_t b, bool filled, bool shadow, int shadow_offset_x, int shadow_offset_y);
void draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint8_t r, uint8_t g, uint8_t b);
void draw_tab();

#endif // __GUI_H__