#include "windows.h"

window_t windows[MAX_WINDOWS];
int num_windows = 0;

void spawn_window(window_t *window) {
  if (num_windows < MAX_WINDOWS) {
    window->buffer = malloc(window->width * window->height * sizeof(uint32_t));
    window->old_buffer =
        malloc(window->width * window->height * sizeof(uint32_t));
    if (window->buffer == NULL || window->old_buffer == NULL) {
      dprintf("Failed to allocate memory for window buffer\n");
      return;
    }

    memset(window->buffer, 0xFFFFFFFF,
           window->width * window->height * sizeof(uint32_t));
    memcpy(window->old_buffer, window->buffer,
           window->width * window->height * sizeof(uint32_t));

    int i, j;
    uint32_t pixel;

    for (i = 0; i < window->height; i++) {
      for (j = 0; j < window->width; j++) {
        pixel = window->buffer[i * window->width + j];
        uint8_t alpha = (pixel & ALPHA_MASK) >> ALPHA_SHIFT;
        uint8_t red = (pixel & RED_MASK) >> RED_SHIFT;
        uint8_t green = (pixel & GREEN_MASK) >> GREEN_SHIFT;
        uint8_t blue = (pixel & BLUE_MASK) >> BLUE_SHIFT;
        put_pixel_rgba(window->x + j, window->y + i, red, green, blue, alpha);
      }
    }

    char *img;
    uint32_t size;
    vfs_op_status status;

    status = driver_read(vfs, 0x00000000, DEFAULT_WIN_DEC, &img);
    if (status == STATUS_OK) {
      size = vfs_get_file_size(vfs, 0x00000000, DEFAULT_WIN_DEC);
      draw_tga_from_raw(window->x, window->y, img, size);
    }

    update_window(window);
  } else {
    dprintf("Maximum number of windows reached\n");
  }
}

void update_window(window_t *window) {
  int i, j;
  uint32_t pixel;

  if (!window->initialized) {
    for (i = 0; i < window->height; i++) {
      for (j = 0; j < window->width; j++) {
        pixel = window->buffer[i * window->width + j];
        uint8_t alpha = (pixel & ALPHA_MASK) >> ALPHA_SHIFT;
        uint8_t red = (pixel & RED_MASK) >> RED_SHIFT;
        uint8_t green = (pixel & GREEN_MASK) >> GREEN_SHIFT;
        uint8_t blue = (pixel & BLUE_MASK) >> BLUE_SHIFT;
        put_pixel_rgba(window->x + j, window->y + i, red, green, blue, alpha);
      }
    }

    char *img;
    uint32_t size;
    vfs_op_status status;

    status = driver_read(vfs, 0x00000000, DEFAULT_WIN_DEC, &img);
    if (status == STATUS_OK) {
      size = vfs_get_file_size(vfs, 0x00000000, DEFAULT_WIN_DEC);
      draw_tga_from_raw(window->x, window->y, img, size);
    }

    window->initialized = 1;
  } else {
    for (i = 0; i < window->height; i++) {
      for (j = 0; j < window->width; j++) {
        pixel = window->buffer[i * window->width + j];
        uint8_t alpha = (pixel & ALPHA_MASK) >> ALPHA_SHIFT;
        uint8_t red = (pixel & RED_MASK) >> RED_SHIFT;
        uint8_t green = (pixel & GREEN_MASK) >> GREEN_SHIFT;
        uint8_t blue = (pixel & BLUE_MASK) >> BLUE_SHIFT;
        put_pixel_rgba(window->x + j, window->y + i, red, green, blue, alpha);
      }
    }

    char *img;
    uint32_t size;
    vfs_op_status status;

    status = driver_read(vfs, 0x00000000, DEFAULT_WIN_DEC, &img);
    if (status == STATUS_OK) {
      size = vfs_get_file_size(vfs, 0x00000000, DEFAULT_WIN_DEC);
      draw_tga_from_raw(window->x, window->y, img, size);
    }
  }
}

void update_all_windows() {
  for (int i = 0; i < num_windows; i++) {
    update_window(&windows[i]);
  }
}
