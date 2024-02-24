#include "gui.h"

void present_element(const gui_element_t *element) {
  if (element == NULL || element->points == NULL) {
    return;
  }

  uint32_t num_points = element->size.x;
  for (uint32_t i = 0; i < num_points; i++) {
    uint32_t next_index = (i + 1) % num_points;
    Vec2 current_point = element->points[i];
    Vec2 next_point = element->points[next_index];
    int32_t x0 = current_point.x;
    int32_t y0 = current_point.y;
    int32_t x1 = next_point.x;
    int32_t y1 = next_point.y;

    int32_t dx = abs(x1 - x0);
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = -abs(y1 - y0);
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;
    int32_t e2;

    while (true) {
      put_pixel_rgba(x0, y0, element->rgba.r, element->rgba.g, element->rgba.b,
                     element->rgba.a);
      if (x0 == x1 && y0 == y1)
        break;
      e2 = 2 * err;
      if (e2 >= dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 <= dx) {
        err += dx;
        y0 += sy;
      }
    }
  }
}
