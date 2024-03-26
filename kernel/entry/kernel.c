/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for
return codes)

*/

// Kernel includes
#include <filesystem/ramdisk.h>
#include <filesystem/tar.h>
#include <filesystem/vfs.h>
#include <entry/init.h>
#include <entry/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/devices/mouse.h>
#include <system/devices/speaker.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>
#include <system/pci/pci.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>
#include <utilities/utilities.h>
#include <x86_64/cpu/panic.h>
#include <x86_64/idt/idt.h>

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>
#include <vector.h>

int kmain()
{
  tty_spawn(1, FONT_BIG);

  vfs_op_status status;
  char* votd = NULL;

  status = driver_read(vfs, 0, "/usr/share/paradox/bible_votd", &votd);

  if(status != STATUS_OK)
    return KERNEL_QUIT_ERROR;

  if (votd != NULL) {
    dprintf("BibleVOTD: \033[1m%s\033[0m\n", votd);
    free(votd);
  }

  keyboard.out = true;

  return KERNEL_QUIT_HANG;
}
