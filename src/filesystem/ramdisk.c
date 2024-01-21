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

  if (result != 0) {
    dprintf("[Ramdisk] Error during tar extraction: %d\n", result);
    return result;
  }

  if (!ramdisk->files) {
    dprintf("[Ramdisk] Failed to allocate memory for ramdisk files\n");
    return 1;
  }

  ramdisk->fileCount = tarCount;

  // for (unsigned int i = 0; i < tarCount; ++i) {
  //   ramdisk->files[i].name = tars[i].files[0].name;
  //   dprintf("[Ramdisk] Added file name\n");
  //   ramdisk->files[i].content = tars[i].files[0].content;
  //   dprintf("[Ramdisk] Added file content\n");
  //   ramdisk->files[i].size = tars[i].files[0].size;
  //   dprintf("[Ramdisk] Added file size\n");
  //   ramdisk->files[i].isDirectory = tars[i].files[0].isDirectory;
  //   dprintf("[Ramdisk] Added file isDirectory flag\n");
  // }

  dprintf("[Ramdisk] Ramdisk initialized successfully\n");

  return 0;
}