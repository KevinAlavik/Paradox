#include "ramdisk.h"
#include <system/memory/memory.h>
#include <strings.h>
#include <printf.h>

int init_ramdisk(struct limine_file *tarfile, struct Ramdisk *ramdisk) {
    const char *rawData = (const char *)tarfile->address;

    unsigned int tarCount = 0;
    struct Tar tars[MAX_TARS];
    extractTarData(rawData, tarfile->size, tars, &tarCount);

    ramdisk->tar = (struct Tar *)malloc(tarCount * sizeof(struct Tar));

    if (!ramdisk->tar) {
       dprintf("Ramdisk has no tars? (Ramdisk Size: %d) (Tar Size: %d) (File ammount: %d)\n", sizeof(ramdisk), sizeof(ramdisk->tar), ramdisk->fileCount);
       return 1;
    }

    ramdisk->fileCount = tarCount;
    return 0;
}