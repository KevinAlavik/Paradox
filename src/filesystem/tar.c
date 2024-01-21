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

int extractTarData(const char *rawData, unsigned int dataSize, struct Tar *tars, unsigned int *tarCount) {
    *tarCount = 0;

    for (unsigned int offset = 0; offset < dataSize && *tarCount < MAX_TARS;) {
        struct TarHeader *header = (struct TarHeader *)(rawData + offset);

        if (header->filename[0] == '\0' || offset > dataSize) {
            dprintf("[TAR] Reached the end of the tar file at offset 0x%x\n", offset);
            break;
        }

        dprintf("[TAR] Processing file at offset 0x%x\n", offset);

        struct File file;
        file.size = strtoul(header->size, NULL, 8);
        file.name = strdup(header->filename);
        file.isDirectory = header->typeflag[0] == '5';

        dprintf("[TAR] File details: Name='%s', Size=0x%x, IsDirectory=%d\n", file.name, file.size, file.isDirectory);

        if (!file.isDirectory) {
            // Load file content
            file.content = (char *)malloc(file.size + 1);
            memcpy(file.content, rawData + offset + 512, file.size);
            file.content[file.size] = '\0';
            dprintf("[TAR] File content: %s\n", file.content);
        } else {
            file.content = NULL;
        }

        struct File *newFiles = (struct File *)malloc((tars[*tarCount].fileCount + 1) * sizeof(struct File));
        if (!newFiles) {
            dprintf("[TAR] Failed to allocate memory for new files\n");
            return 1;
        }

        memcpy(newFiles, tars[*tarCount].files, tars[*tarCount].fileCount * sizeof(struct File));

        free(tars[*tarCount].files);
        tars[*tarCount].files = newFiles;
        tars[*tarCount].files[tars[*tarCount].fileCount] = file;
        tars[*tarCount].fileCount++;

        dprintf("[TAR] File added to the tar structure\n");

        offset += ((file.size + 511) / 512 + 1) * 512;

        if (header->typeflag[0] == '5') {
            // For directories, add an empty file entry
            struct File dirFile;
            dirFile.size = 0;
            dirFile.name = strdup("");
            dirFile.isDirectory = 1;
            dirFile.content = NULL;

            tars[*tarCount].files = (struct File *)realloc(tars[*tarCount].files, (tars[*tarCount].fileCount + 1) * sizeof(struct File));

            if (!tars[*tarCount].files) {
                dprintf("[TAR] Failed to reallocate memory for the directory file\n");
                return 1;
            }

            tars[*tarCount].files[tars[*tarCount].fileCount] = dirFile;
            tars[*tarCount].fileCount++;

            dprintf("[TAR] Incrementing tar count to 0x%x\n", *tarCount);
        }

        // Increment tar count outside the if condition
        (*tarCount)++;
    }

    dprintf("[TAR] Tar parsing is done!\n");

    return 0;
}
