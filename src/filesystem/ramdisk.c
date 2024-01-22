#include "ramdisk.h"
#include <printf.h>
#include <strings.h>
#include <system/memory/memory.h>

int init_ramdisk(struct limine_file *tarfile, struct Ramdisk *ramdisk) {
  const char *rawData = (const char *)tarfile->address;

  unsigned int tarCount = 0;
  struct Tar tars[MAX_TARS];

  dprintf("[Ramdisk] Start initializing ramdisk\n");

  int result = extractTarData(rawData, tarfile->size, tars, &tarCount);

  dprintf("[Ramdisk] Ramdisk initialized successfully\n");

  ramdisk->files = &tars->files;
  ramdisk->fileCount = &tars->fileCount;

  return 0;
}