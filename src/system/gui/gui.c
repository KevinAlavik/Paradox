#include "gui.h"

#include <kif.h>
#include <printf.h>
#include <math.h>
#include <transform.h>

#include <kernel/boot.h>
#include <system/drivers/keyboard.h>
#include <nighterm/nighterm.h>
#include <system/memory/pmm.h>

void init_wm(char *wallpaperData)
{
    keyboard.out = 0;

    draw_image(wallpaperData, framebuffer->width / 2, framebuffer->height / 2, 1);
    draw_tab();
    term.cy = (framebuffer->height / term.font_header.height) - 1;

    printf("Paradox v0.1.1 - %dMB", bytes_to_megabytes(free_memory));
}

void draw_tab()
{
    int tab_height = 30;
    int shadow_offset_x = 2;
    int shadow_offset_y = 4;

    int tab_color_r = 193;
    int tab_color_g = 193;
    int tab_color_b = 193;

    nighterm_set_char_bg(tab_color_r, tab_color_g, tab_color_b);
    nighterm_set_char_fg(0, 0, 0);

    draw_rect(0, framebuffer->height - tab_height, framebuffer->width, tab_height, tab_color_r, tab_color_g, tab_color_b, true, true, shadow_offset_x, shadow_offset_y);
}

void putpixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b)
{
    *(uint32_t *)(framebuffer->address + x * (framebuffer->bpp >> 3) + y * framebuffer->pitch) = (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void draw_filled_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t r, uint8_t g, uint8_t b)
{
    for (uint64_t i = x; i < x + width; i++)
    {
        for (uint64_t j = y; j < y + height; j++)
        {
            putpixel(i, j, r, g, b);
        }
    }
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t r, uint8_t g, uint8_t b, bool filled, bool shadow, int shadow_offset_x, int shadow_offset_y)
{
    if (shadow)
    {
        int shadow_x = x + shadow_offset_x;
        int shadow_y = y + shadow_offset_y;

        // Adjust shadow offset if shadow_y is negative
        if (shadow_y < 0)
        {
            height += shadow_y; // Reduce height to accommodate negative shadow_y
            shadow_y = 0;       // Set shadow_y to 0 since it cannot be drawn above the top edge
        }

        draw_filled_rect(shadow_x, shadow_y, width, height, 0, 0, 0); // Draw shadow
    }

    if (filled)
    {
        draw_filled_rect(x, y, width, height, r, g, b); // Fill rectangle
    }
    else
    {
        draw_line(x, y, x + width - 1, y, r, g, b);                           // Top
        draw_line(x + width - 1, y, x + width - 1, y + height - 1, r, g, b);  // Right
        draw_line(x, y, x, y + height - 1, r, g, b);                          // Left
        draw_line(x, y + height - 1, x + width - 1, y + height - 1, r, g, b); // Bottom
    }
}

void draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint8_t r, uint8_t g, uint8_t b)
{
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    int xIncrement = DIV_ROUND_UP(dx, steps);
    int yIncrement = DIV_ROUND_UP(dy, steps);
    int x = x1, y = y1;

    for (int64_t i = 0; i <= steps; i++)
    {
        putpixel((uint64_t)x, (uint64_t)y, r, g, b);
        x += xIncrement;
        y += yIncrement;
    }
}