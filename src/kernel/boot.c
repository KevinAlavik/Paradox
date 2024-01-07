#include <kernel/boot.h>

#include <limine.h>
#include <system/idt/idt.h>
#include <system/cpu/cpu.h>
#include <system/memory/memory.h>
#include <system/logger/sys_log.h>
#include <system/utilities/utilities.h>

#include <printf.h>

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0};

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 1};

void init_boot()
{
    static struct limine_framebuffer *framebuffer;

    struct limine_file *font = mod_request.response->modules[0];
    framebuffer = framebuffer_request.response->framebuffers[0];

    int nstatus = nighterm_initialize(font->address,
                                      framebuffer->address,
                                      framebuffer->width,
                                      framebuffer->height,
                                      framebuffer->pitch,
                                      framebuffer->bpp,
                                      malloc);
    log(OK, "Initiaized Nighterm");
    init_idt();
    log(OK, "Initiaized IDT");
    init_physical_memory();
    log(OK, "Initiaized PMM");
}