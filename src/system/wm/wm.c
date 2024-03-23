#include "wm.h"

#define MAX_FILE_PATH_LENGTH 100

bool should_draw_cursor = true;

uint32_t old_mouse_pixels[50][50];
uint32_t old_mouse_x = 0;
uint32_t old_mouse_y = 0;
const char *current_mouse_style = "normal";
char *mouse_img = NULL;
uint32_t mouse_img_size = 0;
bool mouse_img_parsed = false;

void init_wallpaper()
{
  char *wallpaper_data;
  vfs_op_status status;
  status = driver_read(vfs, 0x00000000, "/etc/theme/wm/wallpaper.tga", &wallpaper_data);
  if (status == STATUS_OK)
  {
    int size = vfs_get_file_size(vfs, 0x0000000, "/etc/theme/wm/wallpaper.tga");
    draw_tga_from_raw(0, 0, wallpaper_data, size);
  }
  else
  {
    dprintf("[\e[0;31mWindow Manger\e[0m] Failed to read \"/etc/theme/wm/wallpaper.tga\"\n");
  }
}

void update_wm()
{
  return;
}

void init_wm()
{
  keyboard.out = true;
  process_mouse_input = true;
  init_wallpaper();

  Window window;
  window.x = 100;
  window.y = 100;
  window.width = 600;
  window.height = 400;

  window_init(&window, window.x, window.y, window.width, window.height);

  draw_window(&window);
}

void window_init(Window *win, int x, int y, int width, int height)
{
  win->x = x;
  win->y = y;
  win->width = width;
  win->height = height;

  win->old_pixels = (uint32_t **)malloc(win->width * sizeof(uint32_t *));
  for (int i = 0; i < win->width; i++)
  {
    win->old_pixels[i] = (uint32_t *)malloc(win->height * sizeof(uint32_t));
  }

  win->buffer = (uint32_t **)malloc(win->width * sizeof(uint32_t *));
  for (int i = 0; i < win->width; i++)
  {
    win->buffer[i] = (uint32_t *)malloc(win->height * sizeof(uint32_t));
  }
  memset(win->buffer, 0, win->width * win->height);
}

void draw_window(Window *win)
{
  for (int i = 0; i < win->width; i++)
  {
    for (int j = 0; j < win->height; j++)
    {
      int pixel_x = win->x + i;
      int pixel_y = win->y + j;

      win->old_pixels[i][j] = *(uint32_t *)(framebuffer->address +
                                            pixel_x * (framebuffer->bpp >> 3) +
                                            pixel_y * framebuffer->pitch);

      int pixel_index = j * i;
      put_pixel32(win->x + i, win->y + j, 0xFFFFFFFF);
    }
  }
}

void destroy_window(Window *win)
{
  if (win == NULL)
  {
    return;
  }

  process_mouse_input = false;

  for (int i = 0; i < win->width; i++)
  {
    for (int j = 0; j < win->height; j++)
    {
      int pixel_x = win->x + i;
      int pixel_y = win->y + j;

      uint32_t old_pixel_color = win->old_pixels[i][j];
      put_pixel32(pixel_x, pixel_y, old_pixel_color);
    }
  }

  for (int i = 0; i < win->width; i++)
  {
    free(win->old_pixels[i]);
  }

  win->old_pixels = NULL;

  process_mouse_input = true;
}

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
    dprintf("[\e[0;31mMouse Handler\e[0m] Error loading cursor image: %s\n", file_path);
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

    char file_path[MAX_FILE_PATH_LENGTH];
    snprintf(file_path, sizeof(file_path), "/etc/theme/cursors/%s.tga", s);

    if (load_cursor_image(file_path))
    {
      current_mouse_style = s;
      mouse_img_parsed = false;
    }
    else
    {
      dprintf("[\e[0;31mMouse Handler\e[0m] Cursor style '%s' not found. Falling back to default.\n", s);
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

    if (tga == NULL)
    {
      dprintf("[\e[0;31mTGA Parser\e[0m] Error parsing TGA image\n");
      return;
    }
  }

  draw_tga(x, y, tga);
  free(tga->data);
  free(tga);
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

      old_mouse_pixels[i][j] = *(uint32_t *)(framebuffer->address +
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

      uint32_t old_pixel = old_mouse_pixels[i][j];

      if (x != 0 && y != 0)
      {
        uint32_t *framebuffer_address = (uint32_t *)(framebuffer->address + pixel_x * (framebuffer->bpp >> 3) + pixel_y * framebuffer->pitch);
        *framebuffer_address = old_pixel;
      }
    }
  }
}

void mouse(int x, int y)
{
  if (!process_mouse_input)
    return;

  mouse_x = x;
  mouse_y = y;

  if (should_draw_cursor && (old_mouse_x != mouse_x || old_mouse_y != mouse_y))
  {
    remove_mouse(old_mouse_x, old_mouse_y);
    draw_mouse(mouse_x, mouse_y);
    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
