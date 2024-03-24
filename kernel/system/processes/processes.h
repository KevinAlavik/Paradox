#ifndef __PROCESSES_H_
#define __PROCESSES_H_

#include <stdint.h>
#include <stdbool.h>
#include <x86_64/idt/idt.h>

#define MAX_PROCESSES 100

struct Process
{
    uint16_t id;
    int_frame_t *context;
};

extern struct Process *processes[MAX_PROCESSES];

void switch_context(uint16_t pid);

void spawn_process(uint16_t pid, void *entry);
void kill_process(uint16_t pid);

#endif // __PROCESSES_H_
