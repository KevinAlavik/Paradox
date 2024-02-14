#ifndef __TAR_H__
#define __TAR_H__

#include <stdint.h>
#include <stddef.h>

#define TAR_BLOCK_SIZE 512
#define TAR_NAME_SIZE 100
#define TAR_PREFIX_SIZE 155

typedef struct
{
  char filename[TAR_NAME_SIZE];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[8];
  char typeflag[1];
  char linkname[TAR_NAME_SIZE];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[TAR_PREFIX_SIZE];
  char padding[12];
} tar_header_t;

typedef struct
{
  char *name;
  char *content;
  uint32_t size;
  int isDirectory;
} file_t;

typedef struct
{
  file_t *files;
  size_t count;
} files_t;

files_t parse_tar(char *rawData);

#endif // __TAR_H__
