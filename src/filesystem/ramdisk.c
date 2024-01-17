#include "ramdisk.h"

#include <filesystem/tar.h>

int init_ramdisk(struct* limine_module ramdisk) {
  uint64_t tarData = parse_tar(ramdisk->address);
  return 0;
}
