#ifndef __VGA_H__
#define __VGA_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void put_pixel_rgba(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b,
                    uint8_t a);
void put_pixel_rgb(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b);
void put_pixel32(uint64_t x, uint64_t y, uint32_t color);
void flush();
void draw_filled_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height,
                      uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height,
               uint8_t r, uint8_t g, uint8_t b, bool filled, bool shadow,
               int shadow_offset_x, int shadow_offset_y, uint8_t a);
void draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint8_t r,
               uint8_t g, uint8_t b, uint8_t a);

#endif // __VGA_H__