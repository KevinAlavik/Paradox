#include "tar.h"
#include <printf.h>
#include <system/memory/memory.h>
#include <strings.h>

void freeTar(struct Tar *tar) {
    for (unsigned int i = 0; i < tar->fileCount; ++i) {
        free(tar->files[i].name);
        free(tar->files[i].content);
    }
    free(tar->files);
}

int extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tar) {
    tar->files = NULL;
    tar->fileCount = 0;

    // Use a fixed-size array to store files
    struct File fixedFiles[MAX_TARS];

    for (unsigned int offset = 0; offset < dataSize && tar->fileCount < MAX_TARS;) {
        struct TarHeader *header = (struct TarHeader *)(rawData + offset);

        if (header->filename[0] == '\0') {
            dprintf("[TAR] Reached the end of the tar file at offset 0x%x\n", offset);
            break;
        }

        dprintf("[TAR] Processing file at offset 0x%x\n", offset);

        struct File file;
        file.size = getsize(header->size);
        file.name = strdup(header->filename);

        file.isDirectory = header->typeflag[0] == '5';

        if (!file.isDirectory) {
            file.content = (char *)malloc(file.size + 1);
            memcpy(file.content, rawData + offset + 512, file.size);
            file.content[file.size] = '\0';
        } else {
            file.content = "<Directory>";
        }

        // Use fixed-size array instead of realloc
        if (tar->fileCount < MAX_TARS) {
            fixedFiles[tar->fileCount] = file;
            tar->fileCount++;
        } else {
            dprintf("[TAR] Maximum file count reached, skipping remaining files\n");
            break;
        }

        offset += ((file.size + 511) / 512 + 1) * 512;
    }

    // Copy the fixed-size array back to the dynamic array
    tar->files = (struct File *)malloc(tar->fileCount * sizeof(struct File));
    memcpy(tar->files, fixedFiles, tar->fileCount * sizeof(struct File));

    // Print file information for debugging
    for (unsigned int i = 0; i < tar->fileCount; ++i) {
        dprintf("[TAR] File details: Name='%s', Size=0x%x, IsDirectory=%d\n",
                tar->files[i].name, tar->files[i].size, tar->files[i].isDirectory);
    }

    return 0;
}
