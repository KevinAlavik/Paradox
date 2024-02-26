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
#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/drivers/mouse.h>
#include <system/drivers/speaker.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>
#include <system/pci/pci.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>
#include <system/utilities/utilities.h>
#include <system/wm/wm.h>

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>
#include <vector.h>

int main() {
  register_pci();
  init_wm();
  return KERNEL_QUIT_HANG;
}