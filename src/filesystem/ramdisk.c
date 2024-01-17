#include "ramdisk.h"

#include <filesystem/tar.h>

int init_ramdisk(struct limine_file *ramdisk) {
  uint64_t tarData = parse_tar((unsigned int)ramdisk->address);
  return 0;
}
