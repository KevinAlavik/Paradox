#include "processes.h"
#include <kernel/boot.h>
#include <system/memory/pmm.h>

struct Process processes[MAX_PROCESSES];

int_frame_t *cur_ctx = NULL;

void switch_context(uint16_t pid)
{
    (void)pid;
}

void spawn_process(uint16_t pid, void *entry)
{
    (void)pid;
    (void)entry;
}

void kill_process(uint16_t pid)
{
    (void)pid;
}