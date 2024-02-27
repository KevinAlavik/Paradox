#ifndef __GUI_H__
#define __GUI_H__

#include <filesystem/vfs.h>
#include <kernel/boot.h>
#include <kif.h>
#include <math.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <system/drivers/keyboard.h>
#include <system/drivers/mouse.h>
#include <system/memory/pmm.h>
#include <system/utilities/utilities.h>
#include <system/wm/windows.h>
#include <tga.h>
#include <transform.h>
#include <vga.h>

extern bool should_draw_cursor;

void init_wm();

#endif // __GUI_H__