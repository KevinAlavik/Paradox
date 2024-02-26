#include "wm.h"

#define DHEIGHT 40

bool should_draw_cursor;

void init_wallpaper() {
  flush(0, 128, 128);
  nighterm_set_bg_color(0, 128, 128);

  char *t;

  vfs_op_status status;

  status = driver_read(vfs, 0x00000000, "/etc/motd", &t);

  if (status == STATUS_OK) {
    printf("%s\n", t);
  }
}

void init_wm() {
  should_draw_cursor = true;
  keyboard.out = false;

  init_wallpaper();

  window_t test_window;
  test_window.title = "Test Window";
  test_window.x = 100;
  test_window.y = 100;
  test_window.width = WIN_WIDTH;
  test_window.height = WIN_HEIGHT;

  spawn_window(&test_window);
}