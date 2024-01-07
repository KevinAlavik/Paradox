#include "src/corelib/printf.h"
#include "src/system/cpu/cpu.h"

void _start(void) {
    printf("Hello, World!\n");
    hcf();
}