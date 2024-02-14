#include "processes.h"
#include <kernel/boot.h>
#include <system/memory/pmm.h>

struct Process processes[MAX_PROCESSES];

int_frame_t *cur_ctx = NULL; 

void switch_context(uint16_t pid) {
    int_frame_t *ctx = processes[pid].context;

}

void spawn_process(uint16_t pid, void *entry)
{
    if (pid >= MAX_PROCESSES)
    {
        return;
    }

    int_frame_t context;

    context = *cur_ctx;
    dprintf("[Process Manager] Spawning proccess (0x%016llx) with PID %u\n", (uint64_t)entry, pid);
    dprintf("[Process Manager] Current CTX:\n");
    context.rip = (uint64_t*)entry;
    context.rsp = (char*)pmm_request_page() + hhdm_offset;
    context.cs = 0x28;
    context.ss = 0x30;

    dprintf("[Process Manager] rax: 0x%.16llx, rbx: 0x%.16llx, rcx: 0x%.16llx, rdx: 0x%.16llx\n", context.rax, context.rbx, context.rcx, context.rdx);
    dprintf("[Process Manager] rsp: 0x%.16llx, rbp: 0x%.16llx, rsi: 0x%.16llx, rdi: 0x%.16llx\n", context.rsp, context.rbp, context.rsi, context.rdi);
    dprintf("[Process Manager] r8:  0x%.16llx, r9:  0x%.16llx, r10: 0x%.16llx, r11: 0x%.16llx\n", context.r8, context.r9, context.r10, context.r11);
    dprintf("[Process Manager] r12: 0x%.16llx, r13: 0x%.16llx, r14: 0x%.16llx, r15: 0x%.16llx\n", context.r12, context.r13, context.r14, context.r15);
    dprintf("[Process Manager] rfl: 0x%.16llx, rip: 0x%.16llx, cs:  0x%.16llx, ss:  0x%.16llx\n", context.rflags, context.rip, context.cs, context.ss);

    struct Process new_process = {
        .initialized = false,
        .id = pid,
        .context = &context,
        .killsignal = 0};

    processes[pid] = new_process;
}

void kill_process(uint16_t pid) {
    struct Process *process = &processes[pid];
    process->killsignal = 1;
    if (process->context != NULL) {
        process->context = NULL;
    }
    process->id = 0;
}