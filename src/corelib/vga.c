#include "vga.h"

#include <kernel/boot.h> // Include to get access to the framebuffer
#include <math.h>

void put_pixel_rgba(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b,
                    uint8_t a) {
  uint32_t *pixel =
      (uint32_t *)(framebuffer->address + x * (framebuffer->bpp >> 3) +
                   y * framebuffer->pitch);
  uint32_t current_color = *pixel;

  uint8_t current_r = (current_color >> 16) & 0xFF;
  uint8_t current_g = (current_color >> 8) & 0xFF;
  uint8_t current_b = current_color & 0xFF;
  uint8_t current_alpha = (current_color >> 24) & 0xFF;

  uint16_t final_alpha = current_alpha + ((255 - current_alpha) * a) / 255;

  uint8_t final_r =
      ((r * a) / 255) + ((current_r * current_alpha * (255 - a)) / (255 * 255));
  uint8_t final_g =
      ((g * a) / 255) + ((current_g * current_alpha * (255 - a)) / (255 * 255));
  uint8_t final_b =
      ((b * a) / 255) + ((current_b * current_alpha * (255 - a)) / (255 * 255));

  *pixel = (final_alpha << 24) | (final_r << 16) | (final_g << 8) | final_b;
}

void put_pixel_rgb(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b) {
  *(uint32_t *)(framebuffer->address + x * (framebuffer->bpp >> 3) +
                y * framebuffer->pitch) =
      (0xFF << 24) | (r << 16) | (g << 8) | b;
}

void put_pixel32(uint64_t x, uint64_t y, uint32_t color) {
  *(uint32_t *)(framebuffer->address + x * (framebuffer->bpp >> 3) +
                y * framebuffer->pitch) = color;
}

void flush(uint8_t r, uint8_t g, uint8_t b) {
  for (int x = 0; x < framebuffer->width; x++) {
    for (int y = 0; y < framebuffer->height; y++) {
      put_pixel_rgb(x, y, r, g, b);
    }
  }
}

// GUI UTILS
void draw_filled_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height,
                      uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  for (uint64_t i = x; i < x + width; i++) {
    for (uint64_t j = y; j < y + height; j++) {
      put_pixel_rgba(i, j, r, g, b, a);
    }
  }
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height,
               uint8_t r, uint8_t g, uint8_t b, bool filled, bool shadow,
               int shadow_offset_x, int shadow_offset_y, uint8_t a) {
  if (shadow) {
    int shadow_x = x + shadow_offset_x;
    int shadow_y = y + shadow_offset_y;

    if (shadow_y < 0) {
      height += shadow_y;
      shadow_y = 0;
    }

    draw_filled_rect(shadow_x, shadow_y, width, height, 0, 0, 0,
                     a); // Draw shadow
  }

  if (filled) {
    draw_filled_rect(x, y, width, height, r, g, b, a); // Fill rectangle
  } else {
    draw_line(x, y, x + width - 1, y, r, g, b, a); // Top
    draw_line(x + width - 1, y, x + width - 1, y + height - 1, r, g, b,
              a);                                   // Right
    draw_line(x, y, x, y + height - 1, r, g, b, a); // Left
    draw_line(x, y + height - 1, x + width - 1, y + height - 1, r, g, b,
              a); // Bottom
  }
}

void draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint8_t r,
               uint8_t g, uint8_t b, uint8_t a) {
  int64_t dx = x2 - x1;
  int64_t dy = y2 - y1;
  int64_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
  int xIncrement = DIV_ROUND_UP(dx, steps);
  int yIncrement = DIV_ROUND_UP(dy, steps);
  int x = x1, y = y1;

  for (int64_t i = 0; i <= steps; i++) {
    put_pixel_rgba(x, y, r, g, b, a);
    x += xIncrement;
    y += yIncrement;
  }
}
