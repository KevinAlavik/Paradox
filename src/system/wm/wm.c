#include "wm.h"

#include <kif.h>
#include <math.h>
#include <printf.h>
#include <transform.h>

#include <kernel/boot.h>
#include <nighterm/nighterm.h>
#include <system/drivers/keyboard.h>
#include <system/memory/pmm.h>
#include <vga.h>

#define DHEIGHT 25

void init_wm() {
  keyboard.out = 0;
  flush(0, 128, 131);
  nighterm_set_bg_color(0, 128, 131);

  draw_filled_rect(0, framebuffer->height - DHEIGHT, framebuffer->width,
                   DHEIGHT, 192, 192, 192, 255); // Main rectangle

  // Draw a shadow rectangle behind the main rectangle
  draw_filled_rect(0, framebuffer->height - DHEIGHT + 2, framebuffer->width,
                   DHEIGHT + 2, 100, 100, 100, 120);

  char *buf;

  vfs_op_status s = driver_read(vfs, 0x00000000, "/root/welcome.txt", &buf);

  if (s == STATUS_OK) {
    printf("%s\n", buf);
  }
}