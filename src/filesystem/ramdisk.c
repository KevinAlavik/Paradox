#include "ramdisk.h"
#include <printf.h>
#include <strings.h>
#include <system/memory/memory.h>
#include <kernel/boot.h>

int init_ramdisk(struct limine_file *tarfile, struct Ramdisk *ramdisk) {
  const char *rawData = (const char *)tarfile->address;

  unsigned int tarCount = 0;
  struct Tar tars[MAX_TARS];

  dprintf("[Ramdisk] Start initializing ramdisk\n");

  int result = extractTarData(rawData, tarfile->size, tars);

  dprintf("[Ramdisk] Ramdisk initialized successfully\n");

  for (int tar = 0; tar < tars->fileCount; tar++) {
    int size = tars->files[tar].size;
  }

  ramdisk->files = &tars->files;
  ramdisk->fileCount = &tars->fileCount;

  return 0;
}