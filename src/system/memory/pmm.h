#ifndef __PMM_H__
#define __PMM_H__

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <limine.h>
#include <printf.h>

#define PAGE_SIZE 4096

extern uint64_t free_memory; 
extern uint8_t *bitmap;
extern uint64_t bitmap_pages;
extern uint64_t bitmap_size;
extern uint64_t free_memory;

void init_pmm();
void update_memory();
void *pmm_request_page();
void *pmm_request_pages(size_t numPages);
void pmm_free(void *ptr);

// MEMORY FUNCTIONS
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif // __PMM_H__
