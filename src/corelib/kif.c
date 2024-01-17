#include "kif.h"

void draw_image(char* rawData, uint64_t x, uint64_t y) {

    uint64_t width = 0, height = 0;
    int i = 0;
    while (rawData[i] != ' ') {
        width = width * 10 + (rawData[i] - '0');
        i++;
    }
    i++;
    while (rawData[i] != '\n') {
        height = height * 10 + (rawData[i] - '0');
        i++;
    }

    i++;

    uint64_t startX = x, startY = y;
    uint64_t currentX = startX, currentY = startY;
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

        nighterm_putpixel(currentX, currentY, r, g, b);
        currentX++;

        if (currentX == startX + width) {
            currentX = startX;
            currentY++;

            if (currentY == startY + height) {
                break;
            }
        }
    }
}
