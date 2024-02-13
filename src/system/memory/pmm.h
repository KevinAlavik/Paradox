#ifndef __PMM_H__
#define __PMM_H__

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <limine.h>
#include <printf.h>

#define PAGE_SIZE 4096

void pmm_init();

#endif // __PMM_H__