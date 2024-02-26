// Credits to https://github.com/asterd-og/BlazarOS
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <kernel/boot.h>
#include <kif.h>
#include <serial/serial.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <system/idt/idt.h>
#include <system/wm/wm.h>
#include <vga.h>

extern uint32_t mouse_x;
extern uint32_t mouse_y;

extern bool mouse_left_pressed;
extern bool mouse_right_pressed;

extern bool mouse_moved;

void mouse_init();
void set_mouse_style(const char *s);

#endif // __MOUSE_H__
