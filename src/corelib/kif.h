#ifndef KIF_H_
#define KIF_H_

#include <stdint.h>

#include <nighterm/backend/vga.h>

#define draw_pixel(x, y, r, g, b) nighterm_putpixel(uint64_t x, uint64_t y, uint8_t r, uint8_t b, uint8_t g)

void draw_image(char* rawData, uint64_t startX, uint64_t startY)

#endif // KIF_H_
