#ifndef __TTY_H__
#define __TTY_H__

#include <nighterm/nighterm.h>
#incluse <stdint.h>

#define MAX_TTYS 255

typedef struct {
  uint8_t id;
  struct nighterm_ctx *context;
} TTY_t;

extern TTY_t* ttys[MAX_TTYS];

int tty_spawn(uint8_t id);
int tty_destroy(uint8_t id);

void tty_switch(uint8_t id);
void tty_write(uint8_t id, char ch);
void tty_flush(uint8_t id);

#endif // __TTY_H__