/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for return codes)

*/

#include <kernel/kernel.h>
#include <kernel/boot.h>
#include <system/processes/processes.h>

#include <kif.h>
#include <printf.h>

int main() {
    draw_image((char*)mod_request.response->modules[1]->address, 0, 0); // Draw bootlogo, a skull for now
    printf("Hello, World!\n");
}