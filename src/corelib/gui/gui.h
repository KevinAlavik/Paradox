#ifndef __GUI_H__
#define __GUI_H__

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <vector.h>
#include <vga.h>

struct gui_element_struct;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} rgba_t;

typedef struct {
  uint8_t id;
  rgba_t rgba;
  Vec2 pos;
  Vec2 size;
  Vec2 *points;

  void (*present_element)(const struct gui_element_struct *element);
} gui_element_t;

void present_element(const gui_element_t *element);

#endif // __GUI_H__
