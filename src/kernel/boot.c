#include <kernel/boot.h>
#include <limine.h>
#include <nighterm/nighterm.h>
#include <system/idt/idt.h>
#include <system/cpu/cpu.h>
#include <system/memory/memory.h>
#include <system/logger/sys_log.h>
#include <system/utilities/utilities.h>
#include <kernel/main.h>
#include <printf.h>

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0};

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 1};

void init_boot(int debug_info)
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
    //nighterm_clear();
    printf("\n");
    printf("Welcome to ");
    printfColor(77, 166, 255, 0, 0, 0, "Paradox\n");
    char* nreturn =  get_nighterm_return_string(nstatus);
    int mreturn = main();

    if(debug_info) {
        printf("Debug Info:\n - %hhu module(s) loaded\n - Nighterm return code: %s\n - Main return code: %d", mod_request.response->module_count, nreturn, mreturn);
    }   
}
