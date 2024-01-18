#ifndef RAMDISK_H_
#define RAMDISK_H_

#include <limine.h>

#include <filesystem/tar.h>

struct Ramdisk
{
  struct Tar *tar;
  unsigned int fileCount;
};

extern struct Ramdisk *ramdisk;

int init_ramdisk(struct limine_file *tarfile);
void clean_ramdisk(struct Tar *tar);

#endif // RAMDISK_H_
