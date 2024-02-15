#include "tar.h"
#include <strings.h>
#include <system/memory/pmm.h>
#include <printf.h>
#include <system/cpu/cpu.h>

files_t parse_tar(char *rawData)
{
    files_t result;
    file_t files[MAX_FILES];                          // Pre-allocated array to hold file_t structures
    char file_contents[MAX_FILES][FILE_CONTENT_SIZE]; // Pre-allocated buffer pool for file contents
    result.files = files;
    result.count = 0;

    size_t offset = 0;

    dprintf("\n---------- Parsing files ----------\n");

    while (rawData[offset] != '\0' && result.count < MAX_FILES)
    {
        dprintf("[Tar] Processing file entry at offset 0x%016llX\n", offset);

        tar_header_t *header = (tar_header_t *)&rawData[offset];

        if (memcmp(header->filename, "", TAR_NAME_SIZE) == 0)
        {
            dprintf("[Tar] End of archive reached.\n");
            break;
        }

        file_t *file = &files[result.count];
        file->name = header->filename;
        file->size = strtol(header->size, NULL, 8);
        file->isDirectory = (header->typeflag[0] == '5');

        dprintf("[Tar] File name: %s\n", file->name);
        dprintf("[Tar] File size: %llu bytes\n", file->size);
        dprintf("[Tar] Is directory: %s\n", file->isDirectory ? "Yes" : "No");

        if (file->size > FILE_CONTENT_SIZE)
        {
            dprintf("[Tar] File size exceeds maximum content size.\n");
            return result;
        }

        // Copy file content from rawData to pre-allocated buffer
        memcpy(file_contents[result.count], &rawData[offset + TAR_BLOCK_SIZE], file->size);
        file_contents[result.count][file->size] = '\0'; // Ensure null-termination
        dprintf("[Tar] File content: %s\n\n", file_contents[result.count]);

        // Point file content to the pre-allocated buffer
        file->content = file_contents[result.count];

        result.count++;

        // Move offset to the next tar block
        offset += (file->size + TAR_BLOCK_SIZE - 1) / TAR_BLOCK_SIZE * TAR_BLOCK_SIZE + TAR_BLOCK_SIZE;
    }

    dprintf("[Tar] Parsing complete. Total files: %llu\n", result.count);
    dprintf("-----------------------------------\n\n");

    return result;
}

file_t *find_file(files_t *filesList, const char *filename)
{
    for (size_t i = 0; i < filesList->count; ++i)
    {
        if (strcmp(filesList->files[i].name, filename) == 0)
        {
            return &(filesList->files[i]);
        }
    }
    return NULL; // File not found
}
