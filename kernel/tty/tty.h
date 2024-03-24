#ifndef __TTY_H__
#define __TTY_H__

#include <nighterm/nighterm.h>
#include <kernel/init.h>
#incluse <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TTYS 255

typedef struct {
  uint8_t id;
  struct nighterm_ctx *context;
} tty;

extern tty* ttys[MAX_TTYS];

int tty_spawn(uint8_t id);
int tty_destroy(uint8_t id);

void tty_switch(uint8_t id);
void tty_write(char ch);
void tty_flush();

#endif // __TTY_H__
