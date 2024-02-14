#ifndef __PROCESSES_H_
#define __PROCESSES_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_PROCESSES 100 

typedef struct {
  uint64_t rip;
  uint64_t rsp;
  uint64_t cs;
  uint64_t ss;
  uint64_t rflags;
} proc_ctx_t;

struct Process {
  bool initialized;
  uint16_t id;
  proc_ctx_t *context;
  uint8_t killsignal;
};

extern struct Process processes[MAX_PROCESSES]; 

proc_ctx_t get_process_context();

void spawn_process(uint16_t pid, void *entry);
void kill_process(struct Process* process);

#endif // __PROCESSES_H_
