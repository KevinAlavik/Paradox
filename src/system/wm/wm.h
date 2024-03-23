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
#include <tga.h>
#include <transform.h>
#include <system/pit/pit.h>
#include <vga.h>
#include <printf.h>

extern bool should_draw_cursor;

void update_wm();
void init_wm();

// Mouse stuff
void mouse(int x, int y);

// Window stuff
typedef struct
{
    int x;
    int y;
    int width;
    int height;
    uint32_t **buffer;
    uint32_t **old_pixels;
} Window;

void window_init(Window *win, int x, int y, int width, int height);
void draw_window(Window *win);
void destroy_window(Window *win);

#endif // __GUI_H__