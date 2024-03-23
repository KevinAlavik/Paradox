#include "pit.h"
#include <kernel/boot.h>
#include <printf.h>
#include <serial/serial.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/pic/pic.h>

uint64_t uptime_secs;
uint16_t uptime_milis;
uint64_t countdown;

void pit_set_divisor(uint16_t divisor)
{
  if (divisor < 100)
    divisor = 100;
  outb8(PIT_DATA, (uint8_t)(divisor & 0x00ff));
  iowait();
  outb8(PIT_DATA, (uint8_t)((divisor & 0xff00) >> 8));
  dprintf("[\e[0;32mPIT\e[0m] Set divisor, Low byte: 0x%08llx, High byte: "
          "0x%08llx\n",
          (uint8_t)(divisor & 0x00ff), (uint8_t)((divisor & 0xff00) >> 8));
  dprintf("[\e[0;32mPIT\e[0m] - 0x%08llx\n", divisor);
}

uint16_t pit_read_count()
{
  uint16_t count = 0;

  __asm__ volatile("cli");

  // al = channel in bits 6 and 7, remaining bits clear
  outb8(PIT_COMMAND, 0b0000000);

  count = inb8(PIT_DATA);       // Low byte
  count |= inb8(PIT_DATA) << 8; // High byte

  dprintf("[\e[0;32mPIT\e[0m] Read count, Low byte: 0x%08llx, High byte: "
          "0x%08llx\n",
          inb8(PIT_DATA), inb8(PIT_DATA) << 8);
  dprintf("[\e[0;32mPIT\e[0m] - 0x%08llx\n", count);

  __asm__ volatile("sti");

  return count;
}

void pit_set_count(uint16_t count)
{
  // Disable interrupts
  __asm__ volatile("cli");

  outb8(PIT_DATA, count & 0xFF);          // Low byte
  outb8(PIT_DATA, (count & 0xFF00) >> 8); // High byte

  dprintf(
      "[\e[0;32mPIT\e[0m] Set count, Low byte: 0x%08llx, High byte: 0x%08llx\n",
      count & 0xFF, (count & 0xFF00) >> 8);
  dprintf("[\e[0;32mPIT\e[0m] - 0x%08llx\n", count);

  __asm__ volatile("sti");
  return;
}

void pit_handler(int_frame_t *frame)
{
  cur_frame = frame;
  // update_wm();
  pit_int();
}

void pit_init()
{
  pit_set_count(0);
  pit_set_divisor(1193182 / 1000);
  countdown = 0;
  uptime_milis = 0;
  uptime_secs = 0;
  irq_register(0, pit_handler);
}

uint64_t pit_get_uptime_secs() { return uptime_secs; }
uint64_t pit_get_uptime_milis() { return uptime_milis; }

void pit_int()
{
  uptime_milis++;
  if (uptime_milis >= 1000)
  {
    uptime_secs++;
    uptime_milis = 0;
  }

  if (countdown != 0)
  {
    countdown--;
  }
}

void pit_sleep(uint64_t millis)
{
  countdown = millis;

  while (countdown != 0)
  {
    __asm__ volatile("hlt");
  }
}