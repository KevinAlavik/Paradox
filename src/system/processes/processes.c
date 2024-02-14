#include "processes.h"
#include <kernel/boot.h>

struct Process processes[MAX_PROCESSES];

void switch_context(proc_ctx_t context) {
    
}

void spawn_process(uint16_t pid, void *entry)
{
    if (pid >= MAX_PROCESSES)
    {
        return;
    }

    proc_ctx_t context;

    context.rip = (uint64_t*)entry;
    context.rsp = (char*)pmm_request_page() + hhdm_offset;
    context.cs = 0x28;
    context.ss = 0x30;
    context.rflags = 0x202;

    struct Process new_process = {
        .initialized = true,
        .id = pid,
        .context = &context,
        .killsignal = 0};

    processes[pid] = new_process;
}

void kill_process(struct Process *process)
{
    process->initialized = false;
    process->killsignal = 1;
}
