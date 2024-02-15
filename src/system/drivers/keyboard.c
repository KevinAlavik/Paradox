#include "keyboard.h"
#include "layouts.h"
#include <printf.h>
#include <serial/serial.h>
#include <system/pic/pic.h>
#include <system/idt/idt.h>

#include <nighterm/nighterm.h>

struct Keyboard keyboard;

void keyboard_handler(int_frame_t *frame)
{
    uint8_t data = inb8(PS2_DATA);

    if (keyboard.data != data)
    {
        keyboard.data = data;
    }

    // dprintf("[Keyboard Handler] - Scancode: %u; String: %s\n", keyboard.data, sv_layout[keyboard.data].normal);

    char *letterString;

    if (keyboard.data == 0)
    {
        dprintf("[Keyboard Handler] Invalid keyboard data!\n");
    }

    if (keyboard.data == 0x2A || keyboard.data == 0x36)
    {
        keyboard.state = KEYBOARD_SHIFT;
    }
    if (keyboard.data == 0x3A)
    {
        keyboard.state = KEYBOARD_CAPS;
    }
    if (keyboard.data == 0xAA || keyboard.data == 0xB6)
    {
        keyboard.state = KEYBOARD_NORMAL;
    }

    // UNCOMMENTED SINCE NEX DONT HAVE A MOVE CURSOR
    // if (keyboard.data == 0x48)
    // {
    //     nighterm_move_cursor(term.curY - 1, term.curX - 1);
    // }
    // if (keyboard.data == 0x50)
    // {
    //     nighterm_move_cursor(term.curY + 1, term.curX - 1);
    // }
    // if (keyboard.data == 0x4B)
    // {
    //     nighterm_move_cursor(term.curY, term.curX - 1);
    // }
    // if (keyboard.data == 0x4D)
    // {
    //     nighterm_move_cursor(term.curY, term.curX + 1);
    // }

    if (keyboard.state == KEYBOARD_NORMAL)
    {
        letterString = sv_layout[data].normal;
    }
    else if (keyboard.state == KEYBOARD_SHIFT)
    {
        letterString = sv_layout[data].shifted;
    }
    else if (keyboard.state == KEYBOARD_CAPS)
    {
        letterString = sv_layout[data].caps;
    }
    else
    {
        dprintf("[Keybord Handler] Keyboard struct has weird state: %u", keyboard.state);
    }

    // TODO: Store old curor location and compare if its not the same if it isnt then clear the old loc

    if ((keyboard.out) && letterString != "")
    {
        printf("%s", letterString);
    }

    i8259_SendEndOfInterrupt(1);
}

void init_keyboard()
{
    keyboard.state = KEYBOARD_NORMAL;
    keyboard.out = 1;

    irq_register(1, keyboard_handler);
}