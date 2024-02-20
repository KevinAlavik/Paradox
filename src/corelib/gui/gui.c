#include "gui.h"

void present_element(const gui_element_t *element) {
  if (element == NULL) {
    return;
  }

  for (uint64_t i = element->pos.x; i < element->pos.x + element->size.x; i++) {
    for (uint64_t j = element->pos.y; j < element->pos.y + element->size.y;
         j++) {
      put_pixel_rgba(i, j, element->rgba.r, element->rgba.g, element->rgba.b,
                     element->rgba.a);
    }
  }
}