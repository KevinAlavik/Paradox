#ifndef TAR_H
#define TAR_H

#define MAX_TARS 100

#include <stdint.h>
#include <stddef.h>

struct TarHeader {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

struct File {
    char *name;
    char *content;
    unsigned int size;
    int isDirectory;
};

struct Tar {
    struct File *files;
    unsigned int fileCount;
};

void freeTar(struct Tar *tar);
int extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tar);

#endif // TAR_H
