#include "keyboard.h"
#include "layouts.h"
#include <printf.h>
#include <serial/serial.h>
#include <system/devices/mouse.h>
#include <x86_64/idt/idt.h>
#include <system/pic/pic.h>

#include <nighterm/nighterm.h>
#include <tty/tty.h>

struct Keyboard keyboard;

void keyboard_handler(int_frame_t *frame)
{
  (void)frame; // We dont need the current frame so :)

  uint8_t data = inb8(PS2_DATA);

  if (keyboard.data != data)
  {
    keyboard.data = data;
  }

  // dprintf("[\e[0;32mKeyboard Handler\e[0m] - Scancode: %u; String: %s\n",
  // keyboard.data, sv_layout[keyboard.data].normal);

  char *letterString;

  if (keyboard.data == 0)
  {
    dprintf("[\e[0;32mKeyboard Handler\e[0m] Invalid keyboard data!\n");
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

  // Uncommented since nighterm doesn't allow us to read the current x and
  // current y values
  //
  // if (keyboard.data == 0x48) {
  //   nighterm_set_cursor_position(term.curY - 1, term.curX - 1);
  // }
  // if (keyboard.data == 0x50) {
  //   nighterm_set_cursor_position(term.curY + 1, term.curX - 1);
  // }
  // if (keyboard.data == 0x4B) {
  //   nighterm_set_cursor_position(term.curY, term.curX - 1);
  // }
  // if (keyboard.data == 0x4D) {
  //   nighterm_set_cursor_position(term.curY, term.curX + 1);
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
    dprintf("[\e[0;31mKeybord Handler\e[0m] Keyboard struct has weird state: %u",
            keyboard.state);
  }

  // TODO: Store old curor location and compare if its not the same if it isnt
  // then clear the old loc

  if ((keyboard.out) && letterString)
  {
      if (strcmp(letterString, "0") == 0) {
          tty_spawn(0, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "1") == 0) {
          tty_spawn(1, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "2") == 0) {
          tty_spawn(2, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "3") == 0) {
          tty_spawn(3, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "4") == 0) {
          tty_spawn(4, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "5") == 0) {
          tty_spawn(5, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "6") == 0) {
          tty_spawn(6, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "7") == 0) {
          tty_spawn(7, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "8") == 0) {
          tty_spawn(8, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else if (strcmp(letterString, "9") == 0) {
          tty_spawn(9, "/usr/share/fonts/Uni3-Terminus20x10.psf");
      } else {
          printf("%s", letterString);
      }
  }

  i8259_SendEndOfInterrupt(1);
}

void init_keyboard()
{
  keyboard.state = KEYBOARD_NORMAL;
  keyboard.out = 1;

  irq_register(1, keyboard_handler);
}
