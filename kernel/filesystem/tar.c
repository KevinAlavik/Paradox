#include "tar.h"
#include <string.h>

#define TAR_DEBUG(fmt, ...)                                                    \
  dprintf("[\e[0;32mTAR Debug\e[0m] running func: %s(" fmt ")\n", __func__,    \
          ##__VA_ARGS__)

unsigned int getsize(const char *in) {
  TAR_DEBUG("in=%s", in);

  unsigned int size = 0;
  unsigned int count = 1;

  for (int j = 11; j > 0; j--, count *= 8)
    size += ((in[j - 1] - '0') * count);

  return size;
}

char *removePrefix(const char *str, const char *prefix) {
  size_t prefix_len = strlen(prefix);
  if (strncmp(str, prefix, prefix_len) == 0) {
    return strdup(str + prefix_len);
  }
  return strdup(str);
}

void extractTarData(const char *rawData, unsigned int dataSize,
                    struct Tar *tar) {
  TAR_DEBUG("rawData=0x%p, dataSize=%u, tar=0x%p", rawData, dataSize, tar);

  tar->files = NULL;
  tar->fileCount = 0;

  for (unsigned int offset = 0; offset < dataSize;) {
    TAR_DEBUG("offset=%u", offset);

    struct TarHeader *header = (struct TarHeader *)(rawData + offset);

    if (header->filename[0] == '\0') {
      dprintf("[\e[0;32mTAR\e[0m] End of TAR archive detected\n");
      break;
    }

    dprintf("[\e[0;32mTAR\e[0m] Header found at offset 0x%016llX\n",
            (uint64_t)offset);

    struct File file;
    file.size = getsize(header->size);
    file.name = removePrefix(header->filename, RAMDISK_PATH_PREFIX);
    file.isDirectory = header->typeflag[0] == '5';

    if (!file.isDirectory) {
      file.content = (char *)malloc(file.size + 1);
      memcpy(file.content, rawData + offset + 512, file.size);
      file.content[file.size] = '\0';
      dprintf("[\e[0;32mTAR\e[0m] File \"%s\" extracted. Size: %u bytes\n",
              file.name, file.size);
    } else {
      file.content = NULL;
      dprintf("[\e[0;32mTAR\e[0m] Directory \"%s\" extracted\n", file.name);
    }

    struct File *temp_files =
        realloc(tar->files, (tar->fileCount + 1) * sizeof(struct File));
    if (temp_files == NULL) {
      dprintf("[\e[0;32mTAR\e[0m] Failed to allocate memory for temp file\n");
      return;
    }
    tar->files = temp_files;
    TAR_DEBUG("tar->files=0x%p, tar->fileCount=%u", tar->files, tar->fileCount);

    tar->files[tar->fileCount] = file;
    TAR_DEBUG("tar->files[%u] = file", tar->fileCount);

    tar->fileCount++;
    TAR_DEBUG("tar->fileCount=%u", tar->fileCount);

    offset += ((file.size + 511) / 512 + 1) * 512;
    TAR_DEBUG("offset=%u", offset);
  }
  TAR_DEBUG("Function execution finished");
}

void freeTar(struct Tar *tar) {
  TAR_DEBUG("tar=0x%p", tar);

  for (unsigned int i = 0; i < tar->fileCount; ++i) {
    free(tar->files[i].name);
    free(tar->files[i].content);
  }
  free(tar->files);
  TAR_DEBUG("Function execution finished");
}
