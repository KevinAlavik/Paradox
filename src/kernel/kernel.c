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
#include <transform.h>

int main()
{   
    uint64_t hhdm_offset = hhdm_request.response->offset;

    draw_image((char*)mod_request.response->modules[1]->address, framebuffer->width/2, framebuffer->height/2, 1);
    char* boot_info = (char*)mod_request.response->modules[2]->address;
    printf("%s, Mode: pre-boot (parked)\n", boot_info);
    printf("Nighterm Info: %s, %dx%d\n", term.title, framebuffer->width / term.font_header.width, framebuffer->height / term.font_header.height);
    printf("Free Memory: %dMB\n", bytes_to_megabytes(free_memory));   
    printf("\n");

    visualize_pmm(term.font_header.height * term.cy, PAGE_SIZE);
    
    printf("åäö");

    hlt();
}