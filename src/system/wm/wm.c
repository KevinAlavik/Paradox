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

void init_wm() {
  keyboard.out = 1;
  flush(0, 0, 0);
}
