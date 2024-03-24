#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#include <filesystem/tar.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  struct Tar *content;
  uint64_t location;
  uint32_t size;
  uint16_t files;
  uint32_t actual_size;
} ramdisk_t;

ramdisk_t *init_rd(void);
struct File *rd_get_file(ramdisk_t *rd, const char *filename);

#endif // __RAMDISK_H__