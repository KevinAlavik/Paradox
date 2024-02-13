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

#include <kif.h>
#include <printf.h>

int main()
{   
    draw_image((char*)mod_request.response->modules[1]->address, framebuffer->width/2, framebuffer->height/2, 1);
    char* boot_info = (char*)mod_request.response->modules[2]->address;
    printf("Boot Info: %s, Mode: pre-boot (parked)\n", boot_info);
    printf("Nighterm Info: %s, %dx%d\n", term.title, framebuffer->width / term.font_header.width, framebuffer->height / term.font_header.height);
    printf("\n");
    printf("pre-boot status codes:\n");
    printf(" - normal: This just waits for the OS to finish up some things.\n");
    printf(" - parked: The OS is parked in pre-boot, waiting for signal to continue booting...\n");
    printf(" - critical: Something failed in the startup process, check logs/debugcon.log if you are running QEMU (./boot.sh)\n\n");

    nighterm_clear();
    // pmm_init();
    hlt();
}