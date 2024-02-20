#ifndef __TAR_H__
#define __TAR_H__

#include <strings.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>

#define FILENAME_SIZE 100
#define MODE_SIZE 8
#define UID_SIZE 8
#define GID_SIZE 8
#define SIZE_SIZE 12
#define MTIME_SIZE 12
#define CHKSUM_SIZE 8
#define TYPEFLAG_SIZE 1

struct TarHeader {
  char filename[FILENAME_SIZE];
  char mode[MODE_SIZE];
  char uid[UID_SIZE];
  char gid[GID_SIZE];
  char size[SIZE_SIZE];
  char mtime[MTIME_SIZE];
  char chksum[CHKSUM_SIZE];
  char typeflag[TYPEFLAG_SIZE];
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

unsigned int getsize(const char *in);
void extractTarData(const char *rawData, unsigned int dataSize,
                    struct Tar *tar);
void freeTar(struct Tar *tar);

#endif /* __TAR_H__ */
