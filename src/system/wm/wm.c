#include "wm.h"

#define DHEIGHT 25

bool should_draw_cursor;

void init_wallpaper() {
  flush(0, 128, 131);
  nighterm_set_bg_color(0, 128, 131);
}

void init_taskbar() {
  draw_filled_rect(0, framebuffer->height - DHEIGHT, framebuffer->width,
                   DHEIGHT, 192, 192, 192, 255);
  draw_filled_rect(0, framebuffer->height - DHEIGHT + 2, framebuffer->width,
                   DHEIGHT + 2, 100, 100, 100, 120);
}

void init_wm() {
  should_draw_cursor = true;
  keyboard.out = false;

  init_wallpaper();
  init_taskbar();

  window_t test_window;
  test_window.title = "Test Window";
  test_window.x = 100;
  test_window.y = 100;
  test_window.width = WIN_WIDTH;
  test_window.height = WIN_HEIGHT;

  spawn_window(&test_window);
}