#include "tar.h"
#include <printf.h>
#include <system/memory/memory.h>
#include <strings.h>

unsigned int getsize(const char *in) {
  unsigned int size = 0;
  unsigned int count = 1;

  for (int j = 11; j > 0; j--, count *= 8)
    size += ((in[j - 1] - '0') * count);

  return size;
}

void extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tar) {
  tar->files = NULL;
  tar->fileCount = 0;

  for (unsigned int offset = 0; offset < dataSize;) {
    struct TarHeader *header = (struct TarHeader *)(rawData + offset);

    if (header->filename[0] == '\0') {
      break;
    }

    struct File file;
    file.size = getsize(header->size);
    file.name = strdup(header->filename);
    file.isDirectory = header->typeflag[0] == '5';

    if (!file.isDirectory) {
      file.content = (char *)malloc(file.size + 1);
      memcpy(file.content, rawData + offset + 512, file.size);
      file.content[file.size] = '\0';
    } else {
      file.content = NULL;
    }

    tar->files =
        realloc(tar->files, (tar->fileCount + 1) * sizeof(struct File));
    tar->files[tar->fileCount] = file;
    tar->fileCount++;

    offset += ((file.size + 511) / 512 + 1) * 512;
  }
}

void printFileContent(const struct File *file) {
  printf("Name: %s\n", file->name);
  printf("Type: %s\n", file->isDirectory ? "Directory" : "File");
  printf("Size: %u bytes\n", file->size);
  if (!file->isDirectory) {
    printf("Content: %s\n\n", file->content);
  } else {
    printf("Content: (Not available for directories)\n\n");
  }
}

void printTarContents(const struct Tar *tar) {
  for (unsigned int i = 0; i < tar->fileCount; ++i) {
    printFileContent(&tar->files[i]);
  }
}

void freeTar(struct Tar *tar) {
  for (unsigned int i = 0; i < tar->fileCount; ++i) {
    free(tar->files[i].name);
    free(tar->files[i].content);
  }
  free(tar->files);
}
