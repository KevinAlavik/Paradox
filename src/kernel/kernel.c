/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for return codes)

*/

#include <kernel/kernel.h>
#include <kernel/boot.h>
#include <system/logger/sys_log.h>
#include <system/processes/processes.h>
#include <system/pit/pit.h>
#include <system/pic/pic.h>
#include <nighterm/nighterm.h>
#include <system/memory/pmm.h>
#include <system/utilities/utilities.h>
#include <system/drivers/speaker.h>
#include <filesystem/tar.h>
#include <stdint.h>

#include <kif.h>
#include <printf.h>
#include <transform.h>

void test_proc()
{
    printf("Hello, from proccess!\n");
}

int main()
{

    draw_image((char *)mod_request.response->modules[1]->address, framebuffer->width / 2, framebuffer->height / 2, 1);
    char *boot_info = (char *)mod_request.response->modules[2]->address;
    printf("%s, Mode: pre-boot (normal)\n", boot_info);
    printf("Nighterm Info: %s, %dx%d\n", term.title, framebuffer->width / term.font_header.width, framebuffer->height / term.font_header.height);
    printf("Free Memory: %dMB\n", bytes_to_megabytes(free_memory));
    printf("\n");
    printf("Copyright - 2024, Kevin Alavik, All rights reserved\n");
    printf("Paradox is licensed under the GPL-v3 license.\n");
    draw_image((char *)mod_request.response->modules[4]->address, 0, term.font_header.height * term.cy, 0);

    pit_sleep(1500);
    nighterm_clear();
    draw_image((char *)mod_request.response->modules[6]->address, framebuffer->width / 2, framebuffer->height / 2, 1);
    files_t files = parse_tar((char *)mod_request.response->modules[5]->address);
    for (size_t i = 0; i < files.count; ++i)
    {
        printf("File Name: %s\n", files.files[i].name);
        printf("File Size: %u bytes\n", files.files[i].size);
        printf("Is Directory: %s\n", files.files[i].isDirectory ? "Yes" : "No");

        if (files.files[i].isDirectory)
        {
            printf("Content: [Directory]\n\n");
        }
        else if (files.files[i].content != NULL)
        {
            printf("Content: %.*s\n\n", files.files[i].size, files.files[i].content);
        }
        else
        {
            printf("Content: [Content Not Retrieved]\n\n");
        }
    }

    for (size_t i = 0; i < files.count; ++i)
    {
        pmm_free((uintptr_t)files.files[i].content);
    }
    pmm_free((uintptr_t)files.files);
    hlt();
}