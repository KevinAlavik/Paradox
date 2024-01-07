#include <kernel/boot.h>

void _start(void) {
    init_boot();
    hcf();
}