#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include <filesystem/vfs.h>
#include <kernel/boot.h>
#include <math.h>
#include <printf.h>
#include <stdlib.h>
#include <tga.h>
#include <vga.h>

#define MAX_WINDOWS 100

#define WIN_WIDTH 688
#define WIN_HEIGHT 417

#define ALPHA_MASK 0xFF000000
#define RED_MASK 0x00FF0000
#define GREEN_MASK 0x0000FF00
#define BLUE_MASK 0x000000FF

#define ALPHA_SHIFT 24
#define RED_SHIFT 16
#define GREEN_SHIFT 8
#define BLUE_SHIFT 0

#define DEFAULT_WIN_DEC "/etc/graphics/window.tga"

typedef struct {
  char *title;
  int x;
  int y;
  int width;
  int height;
  uint32_t *buffer;
  uint32_t *old_buffer;
  int initialized;
} window_t;

extern window_t windows[MAX_WINDOWS];
extern int num_windows;

void spawn_window(window_t *window);
void update_window(window_t *window);
void update_all_windows();
#endif // __WINDOWS_H__
