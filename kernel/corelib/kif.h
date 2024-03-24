#ifndef KIF_H_
#define KIF_H_

#include <stdint.h>
#include <vga.h>

void draw_image(char *rawData, uint64_t x, uint64_t y, int mode);

#endif // KIF_H_
