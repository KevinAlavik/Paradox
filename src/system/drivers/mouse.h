// Credits to https://github.com/asterd-og/BlazarOS
#ifndef __MOUSE_H__
#define __MOUSE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <vga.h>
#include <system/idt/idt.h>
#include <serial/serial.h>

extern uint32_t mouse_x;
extern uint32_t mouse_y;

extern bool mouse_left_pressed;
extern bool mouse_right_pressed;

extern bool mouse_moved;

void mouse_init();

#endif // __MOUSE_H__