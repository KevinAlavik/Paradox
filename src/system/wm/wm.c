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

  int nstatus = nighterm_initialize(
      NULL, (uint64_t)test_window.buffer, test_window.width, test_window.height,
      test_window.width * 4, framebuffer->bpp, malloc);

  if (nstatus) {
    dprintf(
        "[\e[0;32mSystem\e[0m] Nightem failed to initialize, got code: %s\n",
        get_nighterm_return_string(nstatus));
    hcf();
  } else {
    dprintf("[\e[0;32mSystem\e[0m] Initialized Nighterm with code: %s\n",
            get_nighterm_return_string(nstatus));
  }

  nighterm_set_bg_color(255, 255, 255);
  nighterm_set_fg_color(0, 0, 0);
  printf("Hello, World!\n");
  printf("Hello, World!\n");
  printf("Hello, World!\n");
}