#include "kif.h"
#include <printf.h>

void draw_image(char* rawData, uint64_t x, uint64_t y) {

    uint64_t width = 0, height = 0;
    int i = 0;

    // Extract width
    while (rawData[i] != ' ') {
        if (rawData[i] == '\0') {
            dprintf("[KIF] Error: Invalid image data - Missing width\n");
            return;
        }
        width = width * 10 + (rawData[i] - '0');
        i++;
    }
    i++;

    // Extract height
    while (rawData[i] != '\n') {
        if (rawData[i] == '\0') {
            dprintf("[KIF] Error: Invalid image data - Missing height\n");
            return;
        }
        height = height * 10 + (rawData[i] - '0');
        i++;
    }

    i++;

    uint64_t startX = x, startY = y;
    uint64_t currentX = startX, currentY = startY;
    uint8_t r, g, b;

    while (rawData[i] != '\0') {
        r = 0; g = 0; b = 0;

        // Extract red component
        while (rawData[i] != ' ') {
            if (rawData[i] == '\0') {
                dprintf("[KIF] Error: Invalid image data - Missing red component\n");
                return;
            }
            r = r * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        // Extract green component
        while (rawData[i] != ' ') {
            if (rawData[i] == '\0') {
                dprintf("[KIF] Error: Invalid image data - Missing green component\n");
                return;
            }
            g = g * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        // Extract blue component
        while (rawData[i] != '\n') {
            if (rawData[i] == '\0') {
                dprintf("[KIF] Error: Invalid image data - Missing blue component\n");
                return;
            }
            b = b * 10 + (rawData[i] - '0');
            i++;
        }
        i++;

        // Put pixel with error handling
        if (currentX < startX + width && currentY < startY + height) {
            nighterm_putpixel(currentX, currentY, r, g, b);
            currentX++;
        } else {
            dprintf("[KIF] Error: Image data exceeds specified dimensions\n");
            return;
        }

        if (currentX == startX + width) {
            currentX = startX;
            currentY++;

            if (currentY == startY + height) {
                break;
            }
        }
    }
    dprintf("[KIF] Success: Successfully drew the image\n");
}