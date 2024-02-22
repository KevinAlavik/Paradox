/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for
return codes)

*/

// Kernel includes
#include <filesystem/ramdisk.h>
#include <filesystem/tar.h>
#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/drivers/speaker.h>
#include <system/logger/sys_log.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>
#include <system/utilities/utilities.h>
#include <system/wm/wm.h>

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>

int main() {
  if (rd == NULL) {
    dprintf("[\e[0;32mRamdisk\e[0m] Failed to initialize ramdisk\n");
    return 1;
  }

  struct File *motd = rd_get_file(rd, "ramdisk/etc/motd");

  if (motd == NULL) {
    dprintf("[\e[0;32mKernel\e[0m] Failed to find motd file\n");
    return KERNEL_QUIT_ERROR;
  }

  printf("%s\n\n", motd->content);
  
  for(int i == 0; i < rd->files; i++) {
    struct File curFile = rd->content->files[i];
    
    printf("%s\n", curFile.name);
    printf("\t%s\n\n", curFile.content);
  }
  
  free(rd->content);
  free(rd);

  return KERNEL_QUIT_HANG;
}
