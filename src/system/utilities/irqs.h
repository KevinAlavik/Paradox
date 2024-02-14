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
    cur_ctx = frame;

    for(uint16_t pid = 0; pid < MAX_PROCESSES; pid++) {
        struct Process* proc = &processes[pid];

        if(!proc->initialized && proc->context != NULL) {
            int_frame_t *ctx = proc->context;
            memcpy(frame, ctx, sizeof(int_frame_t));
            dprintf("[Process Manager] Loaded process %u\n", pid);
            proc->initialized = true;
        }
    }
	pit_int();
}

void register_irqs() {
    irq_register(0, pit_handler);
}

#endif // __IRQS_H_