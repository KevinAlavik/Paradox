#include <entry/init.h>
#include <printf.h>
#include <x86_64/cpu/cpu.h>
#include <x86_64/idt/idt.h>
#include <system/memory/pmm.h>

void _start(void)
{
  init_boot(1);
  dprintf("[\e[0;32mSystem\e[0m] Oopsie something broke, init_boot got exited. "
          "When it shouldnt have\n");
  shutdown();
}