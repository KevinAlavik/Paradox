#include "windows.h"

window_t windows[MAX_WINDOWS];
int num_windows = 0;

uint32_t w_old_pixels[MAX_WINDOWS][WIN_WIDTH][WIN_HEIGHT];
int old_window_x[MAX_WINDOWS];
int old_window_y[MAX_WINDOWS];
int last_updated_window_index = -1;

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

    windows[num_windows] = *window;
    num_windows++;

    // // TODO: Make this not hardcoded to window id 0?
    old_window_x[0] = window->x;
    old_window_y[0] = window->y;
    window->initialized = false;

    render_window_gui(window);
    update_window(window);
  } else {
    dprintf("Maximum number of windows reached\n");
  }
}

void render_window_gui(window_t *window) {
  bool buffer_changed = false;

  for (int i = 0; i < window->height; i++) {
    for (int j = 0; j < window->width; j++) {
      if (window->buffer[i * window->width + j] !=
          window->old_buffer[i * window->width + j]) {
        buffer_changed = true;
        break;
      }
    }
    if (buffer_changed) {
      break;
    }
  }

  if (!window->initialized || buffer_changed) {
    for (int i = 0; i < window->height; i++) {
      for (int j = 0; j < window->width; j++) {
        uint32_t pixel = window->buffer[i * window->width + j];
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

    memcpy(window->old_buffer, window->buffer,
           window->width * window->height * sizeof(uint32_t));
    window->initialized = true;
  }
}

bool mouse_style_changed = false;

void update_window(window_t *window) {
  bool buffer_changed = false;
  bool should_set_style = false;

  if ((int32_t)mouse_x >= window->x &&
      (int32_t)mouse_x < window->x + window->width &&
      (int32_t)mouse_y >= window->y &&
      (int32_t)mouse_y < window->y + window->height) {
    if (!mouse_style_changed) {
      should_set_style = true;
      mouse_style_changed = true;
    }
  } else {
    if (mouse_style_changed) {
      should_set_style = true;
      mouse_style_changed = false;
    }
  }

  if (should_set_style) {
    if (mouse_style_changed) {
      set_mouse_style("hand");
    } else {
      set_mouse_style("normal");
    }
  }

  if (old_window_x[0] != window->x || old_window_y[0] != window->y) {
    remove_window_gui(window);
    render_window_gui(window);
    old_window_x[0] = window->x;
    old_window_y[0] = window->y;
  }
  render_window_gui(window);
}

void update_all_windows() {
  for (int i = 0; i < num_windows; i++) {
    update_window(&windows[i]);
  }
}

void remove_window_gui(window_t *window) {
  for (int i = 0; i < window->height; i++) {
    for (int j = 0; j < window->width; j++) {
      int pixel_x = old_window_x[0] + j;
      int pixel_y = old_window_y[0] + i;

      if (pixel_x >= 0 && (uint32_t)pixel_x < framebuffer->width &&
          pixel_y >= 0 && (uint32_t)pixel_y < framebuffer->height) {
        uint32_t old_pixel = w_old_pixels[0][i][j];
        uint32_t *framebuffer_address =
            (uint32_t *)(framebuffer->address +
                         pixel_x * (framebuffer->bpp >> 3) +
                         pixel_y * framebuffer->pitch);
        *framebuffer_address = old_pixel;
      }
    }
  }
}

void destroy_window(window_t *window) {
  remove_window_gui(window);
  free(window->buffer);
  free(window->old_buffer);

  int index = -1;
  for (int i = 0; i < num_windows; i++) {
    if (&windows[i] == window) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    for (int i = index; i < num_windows - 1; i++) {
      windows[i] = windows[i + 1];
    }
    num_windows--;
  }
}