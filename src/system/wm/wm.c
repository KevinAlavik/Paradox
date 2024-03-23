#include "wm.h"
#include <stdlib.h>

bool should_draw_cursor;

void init_wallpaper()
{
  flush(0, 128, 128);
  nighterm_set_bg_color(0, 128, 128);
}

void update_wm()
{
  return;
}

void init_wm()
{
  should_draw_cursor = true;
  keyboard.out = true;
  init_wallpaper();

  Window window;
  window.x = 100;
  window.y = 100;
  window.width = 600;
  window.height = 400;

  window_init(&window, window.x, window.y, window.width, window.height);

  draw_window(&window);
  pit_sleep(5000);
  destroy_window(&window);
}

// Window Stuff
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
    }
  }

  draw_filled_rect(win->x, win->y, win->width, win->height, 0, 0, 0, 255);
}

void destroy_window(Window *win)
{
  if (win == NULL)
  {
    return;
  }

  disable_mouse();

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
  enable_mouse();
}