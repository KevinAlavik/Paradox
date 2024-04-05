#include "tar.h"
#include <string.h>

unsigned int getsize(const char *in)
{
  unsigned int size = 0;
  unsigned int count = 1;

  for (int j = 11; j > 0; j--, count *= 8)
    size += ((in[j - 1] - '0') * count);

  return size;
}

char *removePrefix(const char *str, const char *prefix)
{
  size_t prefix_len = strlen(prefix);
  if (strncmp(str, prefix, prefix_len) == 0)
  {
    return strdup(str + prefix_len);
  }
  return strdup(str);
}

void extractTarData(const char *rawData, unsigned int dataSize,
                    struct Tar *tar)
{

  tar->files = NULL;
  tar->fileCount = 0;

  for (unsigned int offset = 0; offset < dataSize;)
  {

    struct TarHeader *header = (struct TarHeader *)(rawData + offset);

    if (header->filename[0] == '\0')
    {
      break;
    }

    struct File file;
    file.size = getsize(header->size);
    file.name = removePrefix(header->filename, RAMDISK_PATH_PREFIX);
    file.isDirectory = header->typeflag[0] == '5';

    if (!file.isDirectory)
    {
      file.content = (char *)malloc(file.size + 1);
      memcpy(file.content, rawData + offset + 512, file.size);
      file.content[file.size] = '\0';
      dprintf("[\e[0;32mTAR\e[0m] File \"%s\" extracted. Size: %u bytes\n",
              file.name, file.size);
    }
    else
    {
      file.content = NULL;
      dprintf("[\e[0;32mTAR\e[0m] Directory \"%s\" extracted\n", file.name);
    }

    struct File *temp_files =
        realloc(tar->files, (tar->fileCount + 1) * sizeof(struct File));
    if (temp_files == NULL)
    {
      dprintf("[\e[0;32mTAR\e[0m] Failed to allocate memory for temp file\n");
      return;
    }

    tar->files = temp_files;
    tar->files[tar->fileCount] = file;
    tar->fileCount++;

    offset += ((file.size + 511) / 512 + 1) * 512;
  }
}

void freeTar(struct Tar *tar)
{
  for (unsigned int i = 0; i < tar->fileCount; ++i)
  {
    free(tar->files[i].name);
    free(tar->files[i].content);
  }
  free(tar->files);
}
