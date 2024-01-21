#ifndef RAMDISK_H
#define RAMDISK_H

#include <filesystem/tar.h>
#include <limine.h>
#include <stdint.h>

struct Ramdisk {
  struct Tar *tar;
  unsigned int fileCount;
};

int init_ramdisk(struct limine_file *tarfile, struct Ramdisk *ramdisk);

#endif /* RAMDISK_H */
