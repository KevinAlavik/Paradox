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

  char *font_data;

  vfs_op_status status;

  status = driver_read(vfs, 0x00000000, DEFAULT_FONT, &font_data);

  if (status != STATUS_OK)
    return 2;
  
  int s = nighterm_initialize(&context,
            font_data,
            framebuffer->address,
            framebuffer->width,
            framebuffer->height,
            framebuffer->pitch,
            framebuffer->bpp,
            malloc, free);
  
  if(s != NIGHTERM_OK)
    return 3;

  temp.id = id;
  temp.context = &context;
  ttys[id] = &temp;
  return 0;
}
