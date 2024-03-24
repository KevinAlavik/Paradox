#include "processes.h"
#include <entry/init.h>
#include <system/memory/pmm.h>

struct Process *processes[MAX_PROCESSES];

void switch_context(uint16_t pid)
{
    if (processes[pid] == NULL || processes[pid]->context == NULL)
    {
        dprintf("[\e[0;31mProcesses\e[0m] Process with PID \"%d\" does not exist or has no context!", pid);
        return;
    }

    if (cur_frame != NULL)
    {
        memcpy(cur_frame, &processes[pid]->context, sizeof(int_frame_t));
    }

    cur_frame = processes[pid]->context;
    memcpy(&processes[pid]->context, cur_frame, sizeof(int_frame_t));
}

void spawn_process(uint16_t pid, void *entry)
{
    if (pid >= MAX_PROCESSES)
    {
        dprintf("[\e[0;31mProcesses\e[0m] PID \"%d\" exceeds maximum allowed value!", pid);
        return;
    }

    if (processes[pid] != NULL)
    {
        dprintf("[\e[0;31mProcesses\e[0m] PID \"%d\" is currently taken!", pid);
        return;
    }

    struct Process *temp = (struct Process *)malloc(sizeof(struct Process));
    if (temp == NULL)
    {
        dprintf("[\e[0;31mProcesses\e[0m] Failed to allocate memory for process!");
        return;
    }

    temp->context = (int_frame_t *)malloc(sizeof(int_frame_t));
    if (temp->context == NULL)
    {
        dprintf("[\e[0;31mProcesses\e[0m] Failed to allocate memory for process context!");
        free(temp);
        return;
    }

    temp->id = pid;
    temp->context->rip = (uint64_t)entry;
    temp->context->rsp = (uint64_t)malloc(1);

    if (temp->context->rsp == 0)
    {
        dprintf("[\e[0;31mProcesses\e[0m] Failed to allocate stack memory for process!");
        free(temp->context);
        free(temp);
        return;
    }

    processes[pid] = temp;
    dprintf("[\e[0;32mProcesses\e[0m] Spawned process with PID \"%d\"!\n", pid);
    switch_context(pid);
}

void kill_process(uint16_t pid)
{
    if (pid >= MAX_PROCESSES)
    {
        dprintf("[\e[0;31mProcesses\e[0m] PID \"%d\" exceeds maximum allowed value!", pid);
        return;
    }

    if (processes[pid] == NULL)
    {
        dprintf("[\e[0;31mProcesses\e[0m] No process found with PID \"%d\"", pid);
        return;
    }

    free(processes[pid]);
    processes[pid] = NULL;
}