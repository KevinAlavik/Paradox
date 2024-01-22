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
    int nstatus;

    framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_file *rdisk = mod_request.response->modules[0];
    dprintf("\033c");

    init_idt();
    dprintf("[System] Initialized IDT\n");
    init_physical_memory();
    dprintf("[System] Initialized PMM\n");

    struct Ramdisk *ramdisk;
    int rstatus = init_ramdisk(rdisk, &ramdisk);

    if (rstatus)
    {
        dprintf("[System] Ramdisk failed to initialize, got status: %d\n", rstatus);
        hcf();
    }

    dprintf("[System] Loaded ramdisk, file count: %d\n", ramdisk->fileCount);

    if (debug_info)
    {
        for (unsigned int i = 0; i < ramdisk->fileCount; i++)
        {
            struct RamdiskFile *file = &(ramdisk->files[i]);
            dprintf("[%s] Size: %d\n", file->name, file->size);
            dprintf("[%s] Directory: %d\n", file->name, file->isDirectory);
            dprintf("[%s] Content: %s\n", file->name, file->content);

            if(file->name == "public/kfont.psf") {
                nstatus = nighterm_initialize(file->content, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
            }
        }
    }

    dprintf("[System] Starting display...\n");

    if (nstatus) {
        dprintf("[System] Nightem failed to initialize, got code: %s", get_nighterm_return_string(nstatus));
        hcf();
    } else {
        dprintf("[System] Initialized Nighterm with code: %s", get_nighterm_return_string(nstatus));
    }
}
