#include "tty.h"

tty* ttys[MAX_TTYS];

uint8_t cur_tty_id;
tty* cur_tty;

int tty_spawn(uint8_t id)
{
  if(ttys[id] != NULL)
    return 1;

  tty temp;
  struct nighterm_ctx context;
  int s = nighterm_initialize(&context,
            NULL,
            framebuffer->address,
            framebuffer->width,
            framebuffer->height,
            framebuffer->pitch,
            framebuffer->bpp,
            malloc, free);
  if(s != NIGHTERM_OK)
    return 2;

  temp.id = id;
  temp.context = &context;
  ttys[id] = &temp;
  return 0;
}
