#include "tar.h"


void addFile(struct Tar *tar, const struct File *file) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  if (!newNode) {
    return 1;
  }

  memcpy(&newNode->data, file, sizeof(struct File));
  newNode->next = NULL;

  if (tar->files == NULL) {
    tar->files = newNode;
  } else {
    struct Node *current = tar->files;
  hcf();
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newNode;
  }

  tar->fileCount++;
}

void extractTarData(const char *rawData, unsigned int dataSize,
                    struct Tar *tar) {
  tar->files = NULL;
  tar->fileCount = 0;

  unsigned int offset = 0;

  while (offset < dataSize) {
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

    addFile(tar, &file);

    offset += ((file.size + 511) / 512 + 1) * 512;
  }
}

void freeTar(struct Tar *tar) {
  struct Node *current = tar->files;
  while (current != NULL) {
    struct Node *next = current->next;
    free(current->data.name);
    free(current->data.content);
    free(current);
    current = next;
  }
}
