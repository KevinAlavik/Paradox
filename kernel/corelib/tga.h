// Credits to https://github.com/asterd-og/BlazarOS
#ifndef __TGA_H__
#define __TGA_H__

#include <stddef.h>
#include <stdint.h>
#include <vga.h>

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t *data;
} tga_info;

tga_info *tga_parse(uint8_t *ptr, uint32_t size);
void draw_tga(uint32_t x, uint32_t y, tga_info *tga);
void draw_tga_from_raw(uint32_t x, uint32_t y, char *raw_data,
                       uint32_t data_size);

#endif /* __TGA_H__ */