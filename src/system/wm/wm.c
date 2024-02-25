#include "wm.h"
#include <kernel/boot.h>
#include <kif.h>
#include <math.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/drivers/keyboard.h>
#include <system/drivers/mouse.h>
#include <system/memory/pmm.h>
#include <tga.h>
#include <transform.h>
#include <vga.h>

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
  keyboard.out = 0;
  init_wallpaper();
  init_taskbar();

  char *img;
  uint32_t size;

  vfs_op_status status;

  status = driver_read(vfs, 0x00000000, "/root/test/test.tga", &img);

  if (status == STATUS_OK) {
    size = vfs_get_file_size(vfs, 0x00000000, "/root/test/test.tga");
    draw_tga_from_raw(0, 0, img, size);
  }

  should_draw_cursor = true;
  mouse_init();
}