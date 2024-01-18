#include "ramdisk.h"

struct Ramdisk *ramdisk;
struct Tar *tar;

int init_ramdisk(struct limine_file *tarfile) {
  extractTarData((const char *)tarfile->address, tarfile->size, &tar);
  // ramdisk->tar = &tar;
  
  // if(ramdisk->tar->fileCount <= 0) {
  //   free(ramdisk->tar);
  //   freeTar(&tar);
  //   return 1;
  // }

  // ramdisk->fileCount = ramdisk->tar->fileCount;
  return 0;
}

void clean_ramdisk() {  
  free(ramdisk->tar);
  freeTar(&tar);
}
