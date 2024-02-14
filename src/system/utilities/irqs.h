#ifndef __IRQS_H__
#define __IRQS_H__

#include <system/pit/pit.h>
#include <system/pic/pic.h>
#include <system/idt/idt.h>
#include <system/cpu/panic.h>
#include <system/cpu/cpu.h>
#include <system/processes/processes.h>
#include <stdbool.h>

#include <printf.h>

void pit_handler(int_frame_t* frame) {
	pit_int();
}

void register_irqs() {
    irq_register(0, pit_handler);
}

#endif // __IRQS_H_