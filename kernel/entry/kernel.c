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

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>
#include <vector.h>

int main()
{
  keyboard.out = false;
  register_pci();
  printf("-------------------------\n\n");
  vfs_op_status status;
  char *motd;
  status = driver_read(vfs, 0, "/etc/motd", &motd);

  if (status != STATUS_OK)
    return KERNEL_QUIT_ERROR;

  printf("%s\n", motd);
  return KERNEL_QUIT_HANG;
}