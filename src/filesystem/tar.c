#include "tar.h"
#include <printf.h>
#include <system/memory/memory.h>
#include <strings.h>

void extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tars, unsigned int *tarCount) {
  *tarCount = 0;

  for (unsigned int offset = 0; offset < dataSize && *tarCount < MAX_TARS;) {
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

    struct File *newFiles = (struct File *)malloc((tars[*tarCount].fileCount + 1) * sizeof(struct File));
    if (!newFiles) {
        return 1;
    }

    memcpy(newFiles, tars[*tarCount].files, tars[*tarCount].fileCount * sizeof(struct File));

    free(tars[*tarCount].files);
    tars[*tarCount].files = newFiles;
    tars[*tarCount].files[tars[*tarCount].fileCount] = file;
    tars[*tarCount].fileCount++;


    offset += ((file.size + 511) / 512 + 1) * 512;

    if (header->typeflag[0] == '5') {
      (*tarCount)++;
    }
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
