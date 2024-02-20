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
#include <system/utilities/irqs.h>
#include <system/drivers/keyboard.h>

volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0};

struct limine_framebuffer *framebuffer;
uint64_t hhdm_offset;
int_frame_t *cur_frame;

void init_boot(int debug_info)
{
    int nstatus;

    hhdm_offset = hhdm_request.response->offset;
    framebuffer = framebuffer_request.response->framebuffers[0];

    dprintf("[System] Initialized kmsg stream.\n");
    init_idt();
    dprintf("[System] Initialized IDT (And IRQ)\n");
    init_pmm();
    dprintf("[System] Initialized PMM\n");
    pit_init();
    dprintf("[System] Initialized PIT\n");
    init_keyboard();
    dprintf("[System] Initialized Keyboard\n");
    register_irqs();
    dprintf("[System] Registered IRQs\n");
    dprintf("\n");
    dprintf("[System] Loaded modules, file count: %d\n", mod_request.response->module_count);

    if (debug_info)
    {
        for (int files = 0; files < (int)mod_request.response->module_count; files++)
        {
            struct limine_file *curFile = mod_request.response->modules[files];
            int size = curFile->size;
            char *path = curFile->path;
            dprintf("[Modules] Loaded %s (%d)!\n", path, size);
        }

        dprintf("[Screen] Width: %d, Height: %d\n", framebuffer->width, framebuffer->height);
        dprintf("\n");
    }
    dprintf("[System] Starting display...\n");
    if (mod_request.response->modules[0]->size == 10741)
    {
        dprintf("[System] Found font!\n");
        dprintf("[System] Initializing Nighterm with font!\n");
        nstatus = nighterm_initialize((char *)mod_request.response->modules[0]->address, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, NULL);
    }
    else
    {
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

    int kstatus = main(); // Launch the kernel

    if (kstatus == KERNEL_QUIT_SUCCESS)
    {
        dprintf("[Kernel Success] Kernel quit successfully, shutting down in 10 seconds!\n");
        pit_sleep(10000);
        shutdown();
    }
    else if (kstatus == KERNEL_QUIT_ERROR)
    {
        dprintf("[Kernel Error] A kernel error occured, check kmesg for rason! Rebooting...\n");
        reboot();
    }
    else if (kstatus == KERNEL_QUIT_CRITICAL)
    {
        panic("[Kernel Panic] Kernel quit with a critical error code, please see kmsg for extra info.\n", *cur_frame);
        dprintf("[Kernel Panic] Kernel quit with a critical error code, the kernel process returned with a 2 that means something went really wrong, and we are shutting down your computer in 10 seconds!\n");
        pit_sleep(10000);
        shutdown();
    }
    else if (kstatus == KERNEL_QUIT_HANG) 
    {
        hlt();
    }
    else
    {
        dprintf("[Kernel Warning] Kernel returned %d\n", kstatus);
    }

    // If we reach this point, something unexpected happened
    dprintf("[Error] Unexpected behavior occurred, shutting down...\n");
    shutdown();
}
