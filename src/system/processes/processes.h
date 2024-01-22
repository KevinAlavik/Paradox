#ifndef __PROCESSES_H_
#define __PROCESSES_H_

#include <stdint.h>
#include <stdbool.h>
#include <system/idt/idt.h>

struct Process {
  bool initialized;
  struct int_frame_t *context;
  uint8_t deadlysignal;
};

extern struct Process* processes;

int create_proccess();

#endif // __PROCESSES_H_
