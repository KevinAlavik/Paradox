#include <filesystem/ramdisk.h>
#include <kernel/boot.h>
#include <limine.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/logger/sys_log.h>
#include <system/memory/memory.h>
#include <system/utilities/utilities.h>

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

void init_boot(int debug_info)
{
    static struct limine_framebuffer *framebuffer;

    framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_file *rdisk = mod_request.response->modules[0];
    dprintf("\033c");

    init_idt();
    dprintf("Initialized IDT\n");
    init_physical_memory();
    dprintf("Initialized PMM\n");

    struct Ramdisk *ramdisk; // Allocate the Ramdisk structure
    int rstatus = init_ramdisk(rdisk, &ramdisk);

    if (rstatus)
    {
        dprintf("Ramdisk failed to initialize, got status: %d\n", rstatus);
        hcf();
    }

    dprintf("\nLoaded ramdisk, file count: %d\n", ramdisk->fileCount);

    for (unsigned int i = 0; i < ramdisk->fileCount; i++)
    {
        struct RamdiskFile *file = &(ramdisk->files[i]);
        dprintf("- File Name: %s\n", file->name);
        dprintf("  - Size: %d\n", file->size);
        dprintf("  - Directory: %d\n", file->isDirectory);
        dprintf("  - Content: %s\n", file->content);
    }

    dprintf("\nAwesome ramdisk!\n");

}
