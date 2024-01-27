#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <kernel/boot.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/logger/sys_log.h>
#include <system/memory/memory.h>
#include <system/utilities/utilities.h>
#include <system/pit/pit.h>
#include <system/pic/pic.h>

struct Ramdisk *ramdisk;

volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

struct limine_framebuffer *framebuffer;

void init_boot(int debug_info)
{
    int nstatus;

    framebuffer = framebuffer_request.response->framebuffers[0];
    struct limine_file *rdisk = mod_request.response->modules[0];
    dprintf("[System] Initialized kmsg stream (simulated).\n");

    dprintf("[System Check] Checking framebuffer...\n");
    if (sizeof(framebuffer) <= 0) {
        dprintf("[Kernel Error] Framebuffer has an invalid size (%d)...\n", sizeof(framebuffer));
        hcf();
    } else if (framebuffer->width <= 0) {
        dprintf("[Kernel Error] Framebuffer width is an invalid ammount (%d)...\n", framebuffer->width);
        hcf();
    } else if (framebuffer->height <= 0) {
        dprintf("[Kernel Error] Framebuffer height is an invalid ammount (%d)...\n", framebuffer->height);
        hcf();
    }
    dprintf("[System Check] OK. Framebuffer was fine (%dx%d)\n", framebuffer->width, framebuffer->height);
    init_idt();
    dprintf("[System] Initialized IDT\n");
    init_physical_memory();
    dprintf("[System] Initialized PMM\n");
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    dprintf("[System] Initialized PIC\n");
    init_pit();
    dprintf("[System] Initialized PIT\n");
    dprintf("\n");
    
    int rstatus = init_ramdisk(rdisk, &ramdisk);
    
    dprintf("\n");

    if (rstatus)
    {
        dprintf("[System] Ramdisk failed to initialize, got status: %d\n", rstatus);
        hcf();
    }

    dprintf("[System] Loaded modules, file count: %d\n", mod_request.response->module_count);
    dprintf("[System] Loaded ramdisk, file count: %d\n\n", ramdisk->fileCount);

    if (debug_info)
    {
        for (unsigned int i = 0; i < ramdisk->fileCount; i++)
        {
            struct RamdiskFile *file = &(ramdisk->files[i]);
            dprintf("[%s] Size: %d\n", file->name, file->size);
            dprintf("[%s] Directory: %d\n", file->name, file->isDirectory);
            dprintf("[%s] Content: %s\n\n", file->name, file->content);
        }
    }
    dprintf("[System] Starting display...\n");

    if (ramdisk->fileCount > 0)
    {
        for (unsigned int i = 0; i < ramdisk->fileCount; i++)
        {
            struct RamdiskFile *file = &(ramdisk->files[i]);
            if (endsWith(file->name, ".psf"))
            {
                dprintf("[System] Found a font at %s\n", file->name);
                dprintf("[System] Using %s as deafult font for nighterm\n", file->name);
                dprintf("[System Check] Checking framebuffer (second time)...\n");
                if (sizeof(framebuffer) <= 0) {
                    dprintf("[Kernel Error] Framebuffer has an invalid size (%d)...\n", sizeof(framebuffer));
                    hcf();
                } else if (framebuffer->width <= 0) {
                    dprintf("[Kernel Error] Framebuffer width is an invalid ammount (%d)...\n", framebuffer->width);
                    hcf();
                } else if (framebuffer->height <= 0) {
                    dprintf("[Kernel Error] Framebuffer height is an invalid ammount (%d)...\n", framebuffer->height);
                    hcf();
                }
                dprintf("[System Check] OK. Framebuffer was fine (%dx%d)\n", framebuffer->width, framebuffer->height);
                nstatus = nighterm_initialize(file->content, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
                if (nstatus == NIGHTERM_FONT_INVALID)
                {
                    dprintf("[System] Accidentally passed an invalid font to Nighterm, trying again...\n");
                    dprintf("[System] Using deafult built in font for Nighterm!\n");
                    nstatus = nighterm_initialize(NULL, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
                }
            }
            else
            {

                dprintf("[System] Found no valid font!\n");
                dprintf("[System] Initializing Nighterm with deafult built in font!\n");
                nstatus = nighterm_initialize(NULL, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
            }
        }
    }
    else
    {
        dprintf("[System] Found no font!\n");
        dprintf("[System] Initializing Nighterm with deafult built in font!\n");
        nstatus = nighterm_initialize(NULL, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
    }

    if (nstatus)
    {
        dprintf("[System] Nightem failed to initialize, got code: %s\n", get_nighterm_return_string(nstatus));
        hcf();
    }
    else
    {
        dprintf("[System] Initialized Nighterm with code: %s\n", get_nighterm_return_string(nstatus));
    }

    int maxRetries = 5;
    int retryCount = 0;
    int kstatus;

    do
    {
        kstatus = main(); // Launch the kernel
        if (kstatus == 0)
        {
            dprintf("[Kernel Success] Kernel quit successfully, shutting down!\n");
            hcf(); // Replace with shutdown later on
        }
        else if (kstatus == 1)
        {
            dprintf("[Kernel Warning] Kernel quit unexpectedly (%d)! Trying to relaunch (%d/%d)\n", kstatus, retryCount + 1, maxRetries);
            if (retryCount + 1 == 5)
            {
                kstatus = main();
            }
        }
        else if (kstatus == 2)
        {
            printf("[Kernel Panic] Kernel panicked and quit, please see kmsg for extra info.\n");
            dprintf("[Kernel Panic] Kernel panicked and quit, the kernel process returned with a 2 that means something went really wrong, and we are shutting down your computer!\n");
            hcf(); // Replace with shutdown later on
        }

        retryCount++;

    } while (retryCount < maxRetries && kstatus == 1);

    if (kstatus == 1)
    {
        dprintf("[Kernel Warning] Kernel quit unexpectedly (We tried to rescue it but failed), shutting down...\n", retryCount);
        hcf(); // Replace with shutdown later on
    }

    hlt(); // This is needed, if you remove this it exits the boot function and continues to a hcf(); and that breaks things
}
