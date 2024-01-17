#include "kif.h"
#include <stdint.h>

void draw_image(char* rawData, uint64_t startX, uint64_t startY) {
    uint64_t x = startX, y = startY;
    uint8_t r, g, b;

    while (rawData[i] != '\0') {
        r = 0; g = 0; b = 0;
        while (rawData[i] != ' ') {
            r = r * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        while (rawData[i] != ' ') {
            g = g * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        while (rawData[i] != '\n') {
            b = b * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        draw_pixel(x, y, r, g, b);
        x++;
    }
}
