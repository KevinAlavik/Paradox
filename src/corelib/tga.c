#include "tga.h"
#include <printf.h>
#include <system/memory/heap.h>

// Scary osdev.wiki type code.

tga_info *tga_parse(uint8_t *ptr, uint32_t size) {
  uint32_t *data;
  int i, j, k, x, y, w = (ptr[13] << 8) + ptr[12], h = (ptr[15] << 8) + ptr[14],
                     o = (ptr[11] << 8) + ptr[10];
  int m = ((ptr[1] ? (ptr[7] >> 3) * ptr[5] : 0) + 18);

  if (w < 1 || h < 1) {
    dprintf("[\e[0;31mTGA\e[0m] Invalid image dimensions!\n");
    return NULL;
  }

  data = (unsigned int *)malloc((w * h + 2) * sizeof(unsigned int));
  if (!data) {
    dprintf("[\e[0;31mTGA\e[0m] Memory allocation failed!\n");
    return NULL;
  }

  switch (ptr[2]) {
  case 1:
    if (ptr[6] != 0 || ptr[4] != 0 || ptr[3] != 0 ||
        (ptr[7] != 24 && ptr[7] != 32)) {
      dprintf("[\e[0;31mTGA\e[0m] Unsupported color depth or format!\n");
      free(data);
      return NULL;
    }
    for (y = i = 0; y < h; y++) {
      k = ((!o ? h - y - 1 : y) * w);
      for (x = 0; x < w; x++) {
        j = ptr[m + k++] * (ptr[7] >> 3) + 18;
        data[2 + i++] = ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) |
                        (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
      }
    }
    break;
  case 2:
    if (ptr[5] != 0 || ptr[6] != 0 || ptr[1] != 0 ||
        (ptr[16] != 24 && ptr[16] != 32)) {
      dprintf("[\e[0;31mTGA\e[0m] Unsupported color depth or format!\n");
      free(data);
      return NULL;
    }
    for (y = i = 0; y < h; y++) {
      j = ((!o ? h - y - 1 : y) * w * (ptr[16] >> 3));
      for (x = 0; x < w; x++) {
        data[2 + i++] = ((ptr[16] == 32 ? ptr[j + 3] : 0xFF) << 24) |
                        (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
        j += ptr[16] >> 3;
      }
    }
    break;
  case 9:
    if (ptr[6] != 0 || ptr[4] != 0 || ptr[3] != 0 ||
        (ptr[7] != 24 && ptr[7] != 32)) {
      dprintf("[\e[0;31mTGA\e[0m] Unsupported color depth or format!\n");
      free(data);
      return NULL;
    }
    y = i = 0;
    for (x = 0; x < w * h && m < size;) {
      k = ptr[m++];
      if (k > 127) {
        k -= 127;
        x += k;
        j = ptr[m++] * (ptr[7] >> 3) + 18;
        while (k--) {
          if (!(i % w)) {
            i = ((!o ? h - y - 1 : y) * w);
            y++;
          }
          data[2 + i++] = ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) |
                          (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
        }
      } else {
        k++;
        x += k;
        while (k--) {
          j = ptr[m++] * (ptr[7] >> 3) + 18;
          if (!(i % w)) {
            i = ((!o ? h - y - 1 : y) * w);
            y++;
          }
          data[2 + i++] = ((ptr[7] == 32 ? ptr[j + 3] : 0xFF) << 24) |
                          (ptr[j + 2] << 16) | (ptr[j + 1] << 8) | ptr[j];
        }
      }
    }
    break;
  case 10:
    if (ptr[5] != 0 || ptr[6] != 0 || ptr[1] != 0 ||
        (ptr[16] != 24 && ptr[16] != 32)) {
      dprintf("[\e[0;31mTGA\e[0m] Unsupported color depth or format!\n");
      free(data);
      return NULL;
    }
    y = i = 0;
    for (x = 0; x < w * h && m < size;) {
      k = ptr[m++];
      if (k > 127) {
        k -= 127;
        x += k;
        while (k--) {
          if (!(i % w)) {
            i = ((!o ? h - y - 1 : y) * w);
            y++;
          }
          data[2 + i++] = ((ptr[16] == 32 ? ptr[m + 3] : 0xFF) << 24) |
                          (ptr[m + 2] << 16) | (ptr[m + 1] << 8) | ptr[m];
        }
        m += ptr[16] >> 3;
      } else {
        k++;
        x += k;
        while (k--) {
          if (!(i % w)) {
            i = ((!o ? h - y - 1 : y) * w);
            y++;
          }
          data[2 + i++] = ((ptr[16] == 32 ? ptr[m + 3] : 0xFF) << 24) |
                          (ptr[m + 2] << 16) | (ptr[m + 1] << 8) | ptr[m];
          m += ptr[16] >> 3;
        }
      }
    }
    break;
  default:
    dprintf("[\e[0;31mTGA\e[0m] Unsupported image type!\n");
    free(data);
    return NULL;
  }
  data[0] = w;
  data[1] = h;

  tga_info *tga = (tga_info *)malloc(sizeof(tga_info));
  if (!tga) {
    dprintf("[\e[0;31mTGA\e[0m] Memory allocation failed!\n");
    free(data);
    return NULL;
  }
  tga->width = w;
  tga->height = h;
  tga->data = (uint32_t *)malloc(w * h * 4);
  if (!tga->data) {
    dprintf("[\e[0;31mTGA\e[0m] Memory allocation failed!\n");
    free(data);
    free(tga);
    return NULL;
  }
  memcpy(tga->data, data + 2, w * h * 4);

  free(data);

  return tga;
}

void draw_tga(uint32_t x, uint32_t y, tga_info *tga) {
  for (uint32_t yy = 0; yy < tga->height; yy++) {
    for (uint32_t xx = 0; xx < tga->width; xx++) {
      uint32_t pixel = tga->data[xx + (yy * tga->width)];

      // Extract ARGB channels
      uint8_t a = (pixel >> 24) & 0xFF;
      uint8_t r = (pixel >> 16) & 0xFF;
      uint8_t g = (pixel >> 8) & 0xFF;
      uint8_t b = pixel & 0xFF;

      put_pixel_rgba(xx + x, yy + y, r, g, b, a);
    }
  }
}

void draw_tga_from_raw(uint32_t x, uint32_t y, char *raw_data,
                       uint32_t data_size) {
  tga_info *tga = tga_parse((uint8_t *)raw_data, data_size);
  if (tga != NULL) {
    draw_tga(x, y, tga);
    free(tga->data);
    free(tga);
  } else {
    dprintf("[\e[0;31mTGA\e[0m] Failed to parse TGA data!\n");
  }
}
