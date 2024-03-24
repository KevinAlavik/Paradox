#include "tty.h"

tty* ttys[MAX_TTYS];

uint8_t cur_tty_id;
tty* cur_tty;

int tty_spawn(uint8_t id)
{
  if(ttys[id] != NULL)
    return 1;

  tty temp;

  
}
