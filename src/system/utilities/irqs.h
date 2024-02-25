#ifndef __IRQS_H__
#define __IRQS_H__

#include <stdbool.h>
#include <system/cpu/cpu.h>
#include <system/cpu/panic.h>
#include <system/idt/idt.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>

#include <printf.h>

void pit_handler(int_frame_t *frame) {
  cur_frame = frame;
  pit_int();
}

void register_irqs() { irq_register(0, pit_handler); }

#endif // __IRQS_H_