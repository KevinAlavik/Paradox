#include <kernel/boot.h>
#include <system/cpu/cpu.h>

void _start(void) {
    init_boot(1);
    hcf();
}