#include <system/cpu/cpu.h>

void hcf() {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void hlt() {
    for (;;) {
        asm ("hlt");
    }
}