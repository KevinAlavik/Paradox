#ifndef TAR_H
#define TAR_H

#include <printf.h>
#include <system/memory/memory.h>
#include <string.h>
#include <system/cpu/cpu.h>

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

struct Node {
  struct File data;
  struct Node *next;
};

struct Tar {
  struct Node *files;
  unsigned int fileCount;
};



void extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tar);

void printFileContent(const struct File *file);

void printTarContents(const struct Tar *tar);

void freeTar(struct Tar *tar);

#endif /* TAR_H */
