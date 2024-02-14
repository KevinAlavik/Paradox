#ifndef __PROCESSES_H_
#define __PROCESSES_H_

#include <stdint.h>
#include <stdbool.h>
#include <system/idt/idt.h>

#define MAX_PROCESSES 100 

struct Process {
    bool initialized;
    uint16_t id;
    int_frame_t *context;
    uint8_t killsignal;
};

extern struct Process processes[MAX_PROCESSES];
extern int_frame_t *cur_ctx; // Declaration of cur_ctx variable

void switch_context(uint16_t pid);

void spawn_process(uint16_t pid, void *entry);
void kill_process(uint16_t pid);

#endif // __PROCESSES_H_
