// Credits to https://github.com/asterd-og/BlazarOS
#include "mouse.h"

uint8_t mouse_state = 0;
uint8_t mouse_bytes[3] = {0, 0, 0};

uint32_t mouse_x = 0;
uint32_t mouse_y = 0;

uint32_t old_pixels[50][50];
uint32_t old_mouse_x = 0;
uint32_t old_mouse_y = 0;

int32_t mouse_wrap_x = 0;
int32_t mouse_wrap_y = 0;

bool mouse_left_pressed = false;
bool mouse_right_pressed = false;

bool mouse_moved = false;

char *current_mouse_style = "normal";
char *mouse_img = NULL;
uint32_t mouse_img_size = 0;
bool mouse_img_parsed = false;

void mouse_wait_write()
{
  while ((inb8(0x64) & 2) != 0)
  {
    ;
  }
}

void mouse_wait_read()
{
  while ((inb8(0x64) & 1) != 1)
  {
    ;
  }
}

void mouse_write(uint8_t value)
{
  mouse_wait_write();
  outb8(0x64, 0xd4);
  mouse_wait_write();
  outb8(0x60, value);
}

uint8_t mouse_read()
{
  mouse_wait_read();
  return inb8(0x60);
}

// Function to load cursor image from file
bool load_cursor_image(const char *file_path)
{
  vfs_op_status status = driver_read(vfs, 0x00000000, file_path, &mouse_img);
  if (status == STATUS_OK)
  {
    mouse_img_size = vfs_get_file_size(vfs, 0x00000000, file_path);
    return true;
  }
  else
  {
    return false;
  }
}

void set_mouse_style(const char *s)
{
  if (strcmp(current_mouse_style, s) != 0 || !mouse_img_parsed)
  {
    if (mouse_img != NULL)
    {
      free(mouse_img);
      mouse_img = NULL;
    }

    char file_path[100];
    snprintf(file_path, sizeof(file_path), "/etc/theme/cursors/%s.tga", s);

    if (load_cursor_image(file_path))
    {
      current_mouse_style = s;
      mouse_img_parsed = false;
    }
    else
    {
      printf("[\e[0;31mMouse Handler\e[0m] Cursor style '%s' not found. "
             "Falling back to default.\n",
             s);
      load_cursor_image("/etc/theme/cursors/normal.tga");
      current_mouse_style = "normal";
      mouse_img_parsed = false;
    }
  }
}

void tga_draw(uint32_t x, uint32_t y, char *raw_data, uint32_t data_size)
{
  tga_info *tga;
  if (!mouse_img_parsed)
  {
    tga = tga_parse((uint8_t *)raw_data, data_size);
    mouse_img_parsed = true;
  }

  if (tga != NULL)
  {
    draw_tga(x, y, tga);
    free(tga->data);
    free(tga);
  }
  else
  {
    dprintf("[\e[0;31mTGA\e[0m] Failed to parse TGA data!\n");
  }
}

void draw_mouse(int x, int y)
{
  if (mouse_img == NULL || mouse_img_size == 0)
  {
    set_mouse_style(current_mouse_style);
  }

  for (int i = 0; i < 50; i++)
  {
    for (int j = 0; j < 50; j++)
    {
      int pixel_x = x + i;
      int pixel_y = y + j;

      old_pixels[i][j] = *(uint32_t *)(framebuffer->address +
                                       pixel_x * (framebuffer->bpp >> 3) +
                                       pixel_y * framebuffer->pitch);
    }
  }

  tga_draw(x, y, mouse_img, mouse_img_size);
}

void remove_mouse(int x, int y)
{
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      int pixel_x = x + i;
      int pixel_y = y + j;

      uint32_t old_pixel = old_pixels[i][j];

      if (x != 0 && y != 0)
      {
        uint32_t *framebuffer_address = (uint32_t *)(framebuffer->address + pixel_x * (framebuffer->bpp >> 3) + pixel_y * framebuffer->pitch);
        *framebuffer_address = old_pixel;
      }
    }
  }
}

void mouse_update(int8_t accel_x, int8_t accel_y)
{
  if (mouse_wrap_x + accel_x <= 0)
  {
    mouse_wrap_x = 0;
    return;
  }
  if (mouse_wrap_y - accel_y <= 0)
  {
    mouse_wrap_y = 0;
    return;
  }
  if (mouse_wrap_x + accel_x > (int32_t)framebuffer->width)
  {
    mouse_wrap_x = framebuffer->width;
    return;
  }
  if (mouse_wrap_y - accel_y > (int32_t)framebuffer->height)
  {
    mouse_wrap_y = framebuffer->height;
    return;
  }

  mouse_wrap_x += accel_x;
  mouse_wrap_y -= accel_y;

  mouse_x = (uint32_t)mouse_wrap_x;
  mouse_y = (uint32_t)mouse_wrap_y;

  if (should_draw_cursor)
  {
    if (old_mouse_x != mouse_x || old_mouse_y != mouse_y)
    {
      remove_mouse(old_mouse_x, old_mouse_y);
      draw_mouse(mouse_x, mouse_y);
      old_mouse_x = mouse_x;
      old_mouse_y = mouse_y;
    }
  }
}

void mouse_handler(int_frame_t *frame)
{
  (void)frame;
  uint8_t byte = inb8(0x64);
  if ((!(byte & 1)) == 1)
  {
    mouse_state = 0;
    return;
  }
  if ((!(byte & 2)) == 0)
  {
    mouse_state = 0;
    return;
  }
  if (!(byte & 0x20))
  {
    mouse_state = 0;
    return;
  }
  switch (mouse_state)
  {
  case 0:
    mouse_wait_read();
    mouse_bytes[0] = mouse_read();
    mouse_state++;
    break;
  case 1:
    mouse_wait_read();
    mouse_bytes[1] = mouse_read();
    mouse_state++;
    break;
  case 2:
    mouse_wait_read();
    mouse_bytes[2] = mouse_read();

    if (mouse_bytes[0] & 0x80 || mouse_bytes[0] & 0x40)
      return;

    mouse_update(mouse_bytes[1], mouse_bytes[2]);

    mouse_left_pressed = (bool)(mouse_bytes[0] & 0b00000001);
    mouse_right_pressed = (bool)((mouse_bytes[0] & 0b00000010) >> 1);

    mouse_state = 0;
    mouse_moved = true;
    break;
  }
}

void mouse_init()
{
  uint8_t data;
  mouse_wait_write();
  outb8(0x64, 0xa8);
  mouse_wait_write();
  outb8(0x64, 0x20);

  mouse_read();
  data = (inb8(0x60) | 2);

  mouse_wait_write();
  outb8(0x64, 0x60);
  mouse_wait_write();
  outb8(0x60, data);

  mouse_write(0xf6);
  mouse_read();
  mouse_write(0xf4);
  mouse_read();

  irq_register(12, mouse_handler);
}
