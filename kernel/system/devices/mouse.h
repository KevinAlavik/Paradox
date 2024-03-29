// Credits to https://github.com/asterd-og/BlazarOS
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <entry/init.h>
#include <kif.h>
#include <serial/serial.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <x86_64/idt/idt.h>
#include <vga.h>

extern uint32_t mouse_x;
extern uint32_t mouse_y;

extern bool process_mouse_input;

extern bool mouse_left_pressed;
extern bool mouse_right_pressed;

extern bool mouse_moved;

void mouse_init();

#endif // __MOUSE_H__
