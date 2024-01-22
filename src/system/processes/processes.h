#ifndef __PROCESSES_H_
#define __PROCESSES_H_

#include <system/idt/idt.h>
#include <stdint.h>

struct Process {
  bool initialized;
  struct int_frame_t context;
  uint8_t deadlysignal;
};

#endif // __PROCESSES_H_