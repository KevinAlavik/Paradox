#include "ramdisk.h"

#include <filesystem/tar.h>


int init_ramdisk(struct limine_file *ramdisk) {
  struct Tar tar;
  extractTarData((const char *)ramdisk->address, ramdisk->size, &tar);
  printTarContents(&tar);
  freeTar(&tar);
  return 0;
}
