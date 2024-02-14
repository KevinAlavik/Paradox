#ifndef __IRQS_H__
#define __IRQS_H__

#include <system/pit/pit.h>
#include <system/pic/pic.h>
#include <system/idt/idt.h>

__attribute__((interrupt)) void pit_handler(void *) {
	pit_int();
	i8259_SendEndOfInterrupt(0x20);
}	

void register_irqs() {
	set_idt_gate(0x20, (uint64_t)&pit_handler, 0x28, 0x8E);
	i8259_Unmask(0);
}

#endif // __IRQS_H_