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

        if (header->filename[0] == '\0' || offset > dataSize || header->filename[0] == '\0') {
            printf("[TAR] Reached the end of the tar file at offset 0x%x\n", offset);
            break;
        }

        printf("[TAR] Processing file at offset 0x%x\n", offset);

        struct File file;
        file.size = strtoul(header->size, NULL, 8);
        file.name = strdup(header->filename);

        // Skip files with blank names
        if (file.name[0] == '\0') {
            printf("[TAR] Skipping file with blank name\n");
            offset += ((file.size + 511) / 512 + 1) * 512;
            continue;
        }

        file.isDirectory = (header->typeflag[0] == '5') || (header->typeflag[0] == 'D'); // Adjust for your tar format

        printf("[TAR] File details: Name='%s', Size=0x%x, IsDirectory=%d\n", file.name, file.size, file.isDirectory);

        if (!file.isDirectory) {
            // Load file content
            file.content = (char *)malloc(file.size + 1);
            memcpy(file.content, rawData + offset + 512, file.size);
            file.content[file.size] = '\0';
            printf("[TAR] File content: %s\n", file.content);
        } else {
            // For directories, content should be NULL
            file.content = NULL;
        }

        struct File *newFiles = (struct File *)malloc((tars[*tarCount].fileCount + 1) * sizeof(struct File));
        if (!newFiles) {
            printf("[TAR] Failed to allocate memory for new files\n");
            return 1;
        }

        memcpy(newFiles, tars[*tarCount].files, tars[*tarCount].fileCount * sizeof(struct File));
        free(tars[*tarCount].files);
        tars[*tarCount].files = newFiles;
        tars[*tarCount].files[tars[*tarCount].fileCount] = file;
        tars[*tarCount].fileCount++;

        printf("[TAR] File added to the tar structure\n");

        offset += ((file.size + 511) / 512 + 1) * 512;

        if (file.isDirectory) {
            // For directories, add an empty file entry
            struct File dirFile;
            dirFile.size = 0;
            dirFile.name = strdup(""); // Empty string for directory names
            dirFile.isDirectory = 1;
            dirFile.content = NULL; // Directory content is NULL

            tars[*tarCount].files = (struct File *)realloc(tars[*tarCount].files, (tars[*tarCount].fileCount + 1) * sizeof(struct File));

            if (!tars[*tarCount].files) {
                printf("[TAR] Failed to reallocate memory for the directory file\n");
                return 1;
            }

            tars[*tarCount].files[tars[*tarCount].fileCount] = dirFile;
            tars[*tarCount].fileCount++;

            printf("[TAR] Incrementing tar count to 0x%x\n", *tarCount);
        }

        (*tarCount)++;
    }

    printf("[TAR] Tar parsing is done!\n");

    return 0;
}
