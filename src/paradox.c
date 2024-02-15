#include <kernel/boot.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/memory/pmm.h>

void _start(void)
{
  init_boot(1);
  dprintf("[System] Oopsie something broke, init_boot got exited. When it shouldnt have\n");
  shutdown();
}