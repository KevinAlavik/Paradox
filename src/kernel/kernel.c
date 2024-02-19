/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for
return codes)

*/

// Kernel includes
#include <filesystem/tar.h>
#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/drivers/speaker.h>
#include <system/gui/gui.h>
#include <system/logger/sys_log.h>
#include <system/memory/pmm.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>
#include <system/utilities/utilities.h>

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>

void test_proc() { printf("Hello, from proccess!\n"); }

int main() {

  draw_image((char *)mod_request.response->modules[1]->address,
             framebuffer->width / 2, framebuffer->height / 2, 1);
  char *boot_info = (char *)mod_request.response->modules[2]->address;
  printf("%s, Mode: pre-boot (normal)\n", boot_info);
  printf("Nighterm Info: %s, %dx%d\n", term.title,
         framebuffer->width / term.font_header.width,
         framebuffer->height / term.font_header.height);
  printf("Free Memory: %dMB\n", bytes_to_megabytes(free_memory));
  printf("\n");
  printf("Copyright - 2024, Kevin Alavik, All rights reserved\n");
  printf("Paradox is licensed under the GPL-v3 license.\n");
  draw_image((char *)mod_request.response->modules[4]->address, 0,
             term.font_header.height * term.cy, 0);

  // nighterm_clear();
  init_wm((char *)mod_request.response->modules[6]->address);
  // TODO:
  //  impelement a real WM
}
