#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/logger/sys_log.h>
#include <system/utilities/utilities.h>
#include <system/pit/pit.h>
#include <system/pic/pic.h>
#include <system/memory/pmm.h>

volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

struct limine_framebuffer *framebuffer;

void init_boot(int debug_info)
{
    int nstatus;
    framebuffer = framebuffer_request.response->framebuffers[0];
    dprintf("[System] Initialized kmsg stream.\n");
    init_idt();
    dprintf("[System] Initialized IDT\n");
    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    dprintf("[System] Initialized PIC\n");
    init_pit();
    dprintf("[System] Initialized PIT\n");
    dprintf("\n");

    dprintf("[System] Loaded modules, file count: %d\n", mod_request.response->module_count);

    if (debug_info)
    {
        for(int files = 0; files < (int)mod_request.response->module_count; files++) {
            struct limine_file* curFile = mod_request.response->modules[files];
            int size = curFile->size;
            char* path = curFile->path;
            dprintf("[Modules] Loaded %s (%d)!\n", path, size);
        }

        dprintf("[Screen] Width: %d, Height: %d\n", framebuffer->width, framebuffer->height);
        dprintf("\n");
    }
    dprintf("[System] Starting display...\n");
    if(mod_request.response->modules[0]->size == 10741)
    {
        dprintf("[System] Found font!\n");
        dprintf("[System] Initializing Nighterm with font!\n");
        nstatus = nighterm_initialize((char*)mod_request.response->modules[0]->address, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, NULL);
    } else {
        dprintf("[Font Error] Failed to get font, no kfont.psf? (Got size: %d)", mod_request.response->modules[0]->size);
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
