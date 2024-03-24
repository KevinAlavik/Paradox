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
#include <kernel/init.h>
#include <kernel/kernel.h>
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

  return KERNEL_QUIT_HANG;
}