#ifndef TAR_H
#define TAR_H

#define MAX_TARS 100

#include <stdint.h>
#include <stddef.h>

struct TarHeader {
    char filename[1000];
    char size[100];
    char typeflag[1];
};

struct File {
    char *name;
    char *content;
    size_t size;
    int isDirectory;
};

struct Tar {
    struct File *files;
    unsigned int fileCount;
};

void freeTar(struct Tar *tar);

int extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tars, unsigned int *tarCount);

#endif // TAR_H
