#include "ramdisk.h"
#include <kernel/boot.h>
#include <printf.h>

#define RD_MODULE_NAME mod_request.response->modules[0]

ramdisk_t *init_rd(void) {
  ramdisk_t *rd = (ramdisk_t *)malloc(sizeof(ramdisk_t));
  if (rd == NULL) {
    dprintf("[\e[0;31mRamdisk\e[0m] Failed to allocate memory for ramdisk_t");
    return NULL;
  }

  struct limine_file *temp_file;
  temp_file = RD_MODULE_NAME;

  struct Tar *tar = (struct Tar *)malloc(sizeof(struct Tar));
  if (tar == NULL) {
    dprintf("[\e[0;31mRamdisk\e[0m] Failed to allocate memory for Tar");
    free(rd);
    return NULL;
  }

  extractTarData((char *)(temp_file->address), (char *)(temp_file->size), tar);

  rd->content = tar; // Assign dynamically allocated tar to rd->content

  if (rd->content == NULL) {
    free(rd); // Free rd if content is NULL
    return NULL;
  }

  rd->location = (uint64_t)rd;
  rd->size = temp_file->size;
  rd->files = rd->content->fileCount;
  rd->actual_size = sizeof(ramdisk_t);

  dprintf("[\e[0;32mRamdisk\e[0m] Ramdisk located at 0x%016llX is now "
          "initialized!\n",
          rd->location);

  return rd;
}

struct File *rd_get_file(ramdisk_t *rd, const char *filename) {
  if (rd == NULL || rd->content == NULL || filename == NULL) {
    return NULL;
  }

  for (unsigned int i = 0; i < rd->content->fileCount; ++i) {
    if (strcmp(rd->content->files[i].name, filename) == 0) {
      return &(rd->content->files[i]);
    }
  }

  return NULL;
}