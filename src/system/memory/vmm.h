#ifndef __VMM_H__
#define __VMM_H__

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <limine.h>
#include <printf.h>

#define PAGE_SIZE 4096

uint64_t page_map[PAGE_SIZE];

void init_vmm();

// MEMORY FUNCTIONS

#endif // __VMM_H__