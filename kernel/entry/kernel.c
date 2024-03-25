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

void terminal_launch()
{
  tty_spawn(1, "/usr/share/fonts/Uni3-Terminus20x10.psf");
  keyboard.out = true;
}

int main()
{
  keyboard.out = false;
  tty_spawn(0, "/usr/share/fonts/Uni3-Terminus12x6.psf");
  register_pci();
  terminal_launch();
  return KERNEL_QUIT_HANG;
}
