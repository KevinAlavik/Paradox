#include "tty.h"

tty* ttys[MAX_TTYS];

uint8_t cur_tty_id;
tty* cur_tty;

VFS_t *vfs_b;
struct limine_framebuffer *framebuffer_b;

void tty_init(VFS_t *vfs, struct limine_framebuffer *framebuffer) {
  vfs_b = vfs;
  framebuffer_b = framebuffer;
}


void tty_flush() {
  nighterm_flush(cur_tty->context, 0, 0, 0);
  nighterm_set_cursor_position(cur_tty->context, 0, 0);
}

int tty_spawn(uint8_t id, char* font_path)
{
  if(ttys[id] != NULL)
    return 1;

  struct nighterm_ctx *context = malloc(sizeof(struct nighterm_ctx));
  if (context == NULL)
    return 4;

  char *font_data;
  vfs_op_status status = driver_read(vfs_b, 0x00000000, font_path, &font_data);

  if (status != STATUS_OK) {
    free(context);
    return 2;
  }

  int s = nighterm_initialize(context,
            font_data,
            framebuffer_b->address,
            framebuffer_b->width,
            framebuffer_b->height,
            framebuffer_b->pitch,
            framebuffer_b->bpp,
            malloc, free);

  free(font_data);

  if(s != NIGHTERM_SUCCESS) {
    free(context);
    return 3;
  }

  tty* temp = malloc(sizeof(tty));
  if (temp == NULL) {
    free(context);
    return 4;
  }

  temp->id = id;
  temp->context = context;
  ttys[id] = temp;
  tty_switch(id);
  return 0;
}

int tty_destroy(uint8_t id) {
  if(ttys[id] == NULL)
    return 1;

  free(ttys[id]->context);
  free(ttys[id]);
  ttys[id] = NULL;

  if (cur_tty_id == id) {
    cur_tty_id = 0;
    cur_tty = NULL;
  }

  return 0;
}


void tty_switch(uint8_t id) {
  cur_tty = ttys[id];
  cur_tty_id = id;
  tty_flush();
}

void tty_write(char ch) {
  struct nighterm_ctx *context = cur_tty->context;
  nighterm_write(context, ch);
}
