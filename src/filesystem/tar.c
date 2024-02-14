#include "tar.h"
#include <strings.h>
#include <system/memory/pmm.h>
#include <printf.h>

#define MAX_FILES 100

files_t parse_tar(char *rawData)
{
    files_t result;
    result.files = NULL;
    result.count = 0;

    size_t offset = 0;
    size_t fileIndex = 0;
    file_t files[MAX_FILES];

    while (rawData[offset] != '\0' && result.count < MAX_FILES)
    {
        dprintf("[Tar] Processing file entry at offset 0x%016llX\n", offset);

        tar_header_t *header = (tar_header_t *)&rawData[offset];

        if (memcmp(header->filename, "", TAR_NAME_SIZE) == 0)
        {
            dprintf("[Tar] End of archive reached.\n");
            break;
        }

        file_t file;
        file.name = header->filename;
        file.size = strtol(header->size, NULL, 8);
        file.isDirectory = (header->typeflag[0] == '5');

        dprintf("[Tar] File name: %s\n", file.name);
        dprintf("[Tar] File size: %llu bytes\n", file.size);
        dprintf("[Tar] Is directory: %s\n", file.isDirectory ? "Yes" : "No");

        file.content = (char *)pmm_request_page();
        if (file.content == NULL)
        {
            dprintf("[Tar] Memory allocation failed.\n");
            return result;
        }

        // Copy file content from rawData to file.content
        memcpy(file.content, &rawData[offset + TAR_BLOCK_SIZE], file.size);
        file.content[file.size] = '\0'; // Ensure null-termination

        dprintf("[Tar] File content: %s\n\n", file.content);

        files[fileIndex++] = file;
        result.count++;

        // Move offset to the next tar block
        offset += (file.size + TAR_BLOCK_SIZE - 1) / TAR_BLOCK_SIZE * TAR_BLOCK_SIZE + TAR_BLOCK_SIZE;
    }

    result.files = (file_t *)pmm_request_page();
    if (result.files == NULL)
    {
        dprintf("[Tar] Memory allocation failed.\n");
        return result;
    }

    for (size_t i = 0; i < result.count; ++i)
    {
        result.files[i].name = files[i].name;
        result.files[i].size = files[i].size;
        result.files[i].isDirectory = files[i].isDirectory;

        result.files[i].content = (char *)pmm_request_page();
        if (result.files[i].content == NULL)
        {
            dprintf("[Tar] Memory allocation failed.\n");
            return result;
        }

        memcpy(result.files[i].content, files[i].content, files[i].size + 1);
    }

    dprintf("[Tar] Parsing complete. Total files: %llu\n", result.count);

    return result;
}
