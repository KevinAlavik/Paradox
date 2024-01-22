#include <kernel/boot.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/memory/memory.h>

void _start(void) {
  init_boot(1);
  hcf();
}