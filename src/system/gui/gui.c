#include "gui.h"

#include <kif.h>
#include <math.h>
#include <printf.h>
#include <transform.h>

#include <kernel/boot.h>
#include <nighterm/nighterm.h>
#include <system/drivers/keyboard.h>
#include <system/memory/pmm.h>
#include <vga.h>

void init_wm(char *wallpaperData) {
  keyboard.out = 0;
  flush();
  draw_image(wallpaperData, 0, 0, 0);
  draw_filled_rect(20, 20, 600, 400, 255, 255, 255, 150);
}
