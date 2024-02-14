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

void switch_context(int_frame_t context);

void spawn_process(uint16_t pid, void *entry);
void kill_process(struct Process* process);

#endif // __PROCESSES_H_
