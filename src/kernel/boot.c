#include <kernel/boot.h>
#include <kernel/kernel.h>
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
    dprintf("\n");

    struct Ramdisk *ramdisk;
    int rstatus = init_ramdisk(rdisk, &ramdisk);
    dprintf("\n");

    if (rstatus)
    {
        dprintf("[System] Ramdisk failed to initialize, got status: %d\n", rstatus);
        hcf();
    }

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

    dprintf("\n----- End of boot proccess -----\n\n");

    int maxRetries = 5;
    int retryCount = 0;
    int kstatus;

    do
    {
        kstatus = main(); // Launch the kernel

        if (kstatus != 0 && kstatus != 22)
        {
            dprintf("[Kernel Warning] Kernel quited unexpectedly (%d)! Trying to relaunch (%d/%d)\n", kstatus, retryCount + 1, maxRetries);
        }

        retryCount++;

    } while (retryCount < maxRetries && (kstatus != 0 && kstatus != 22));

    dprintf("\n---------------------------------\n");

    if (kstatus != 22)
    {
        if (retryCount >= maxRetries)
        {
            printf("[Kernel Error] Failed to launch the kernel after %d attempts. Please shutdown your computer!\n", maxRetries);
            dprintf("Kernel quited with an unexpected reason (%d) and couldn't be relaunched.\n", kstatus);
            hcf();
        }
        else
        {
            printf("[Kernel Warning] Kernel quited unexpectedly (see debug log)! Please shutdown your computer!\n"); // Display warning for the user to shutdown
            dprintf("Kernel quited with an unexpected reason (%d) after %d attempts.\n", kstatus, retryCount);
        }
    }

    hlt();
}
