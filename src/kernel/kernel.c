/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for return codes)

*/

#include <kernel/kernel.h>
#include <kernel/boot.h>
#include <filesystem/ramdisk.h>
#include <system/processes/processes.h>
#include <nighterm/nighterm.h>

#include <kif.h>
#include <printf.h>

int main()
{
    dprintf("Hello, World!\n");
}