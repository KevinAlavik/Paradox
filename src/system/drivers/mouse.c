// Credits to https://github.com/asterd-og/BlazarOS
#include "mouse.h"

uint8_t mouse_state = 0;
uint8_t mouse_bytes[3] = {0, 0, 0};

uint32_t mouse_x = 0;
uint32_t mouse_y = 0;

int32_t mouse_wrap_x = 0;
int32_t mouse_wrap_y = 0;

bool mouse_left_pressed = false;
bool mouse_right_pressed = false;

bool mouse_moved = false;

void mouse_wait_write() {
  while ((inb8(0x64) & 2) != 0) {
    ;
  }
}

void mouse_wait_read() {
  while ((inb8(0x64) & 1) != 1) {
    ;
  }
}

void mouse_write(uint8_t value) {
  mouse_wait_write();
  outb8(0x64, 0xd4);
  mouse_wait_write();
  outb8(0x60, value);
}

uint8_t mouse_read() {
  mouse_wait_read();
  return inb8(0x60);
}

void mouse_update(int8_t accel_x, int8_t accel_y) {
  if (mouse_wrap_x + accel_x <= 0) {
    mouse_wrap_x = 0;
    return;
  }
  if (mouse_wrap_y - accel_y <= 0) {
    mouse_wrap_y = 0;
    return;
  }
  if (mouse_wrap_x + accel_x > (int32_t)framebuffer->width) {
    mouse_wrap_x = framebuffer->width;
    return;
  }
  if (mouse_wrap_y - accel_y > (int32_t)framebuffer->height) {
    mouse_wrap_y = framebuffer->height;
    return;
  }

  mouse_wrap_x += accel_x;
  mouse_wrap_y -= accel_y;

  mouse_x = (uint32_t)mouse_wrap_x;
  mouse_y = (uint32_t)mouse_wrap_y;

  if (should_draw_cursor) {
    char *img;
    uint32_t size;

    vfs_op_status status;

    status =
        driver_read(vfs, 0x00000000, "/etc/graphics/cursor_normal.tga", &img);

    if (status == STATUS_OK) {
      size =
          vfs_get_file_size(vfs, 0x00000000, "/etc/graphics/cursor_normal.tga");
      draw_tga_from_raw(mouse_x, mouse_y, img, size);
    }
  }
}

void mouse_handler(int_frame_t *frame) {
  (void)frame;
  uint8_t byte = inb8(0x64);
  if ((!(byte & 1)) == 1) {
    mouse_state = 0;
    return;
  }
  if ((!(byte & 2)) == 0) {
    mouse_state = 0;
    return;
  }
  if (!(byte & 0x20)) {
    mouse_state = 0;
    return;
  }
  switch (mouse_state) {
  // Packet state
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

void mouse_init() {
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