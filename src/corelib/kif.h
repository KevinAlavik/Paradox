#ifndef KIF_H_
#define KIF_H_

#include <stdint.h>

#define draw_pixel(x, y, r, g, b) plot_pixel(x, y, r, g, b)

// ^^^ Change the plot_pixel function to your draw thing

void draw_image(char* rawData, uint64_t startX, uint64_t startY)

#endif // KIF_H_
