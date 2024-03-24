// Credits to
// https://github.com/roidsos/h0r.net/blob/rewrite/src/core/Memory/Heap.c

#include "heap.h"
#include <printf.h>
#include <system/memory/pmm.h>

void *malloc(size_t size) {
  char *PP = (char *)pmm_request_pages((size / PAGE_SIZE) + 1);
  if (PP == NULL) {
    dprintf("[\e[0;31mHeap Error\e[0m] Failed to allocate memory\n");
    return NULL; // Memory allocation failed
  }
  *((size_t *)PP) = size;
  PP += sizeof(size_t); // Note: this value is architecture dependent
  return (void *)PP;
}

void free(void *tofree) {
  if (tofree == NULL) {
    return; // Nothing to free
  }
  char *PP = (char *)tofree;
  PP -= sizeof(size_t); // Note: this value is architecture dependent
  size_t size = *((size_t *)PP);
  pmm_free_pages(PP, (size / PAGE_SIZE) + 1);
}

void *calloc(size_t size) {
  void *mallocVal = malloc(size);
  if (mallocVal == NULL) {
    return NULL; // Memory allocation failed
  }
  memset(mallocVal, 0, size);
  return mallocVal;
}

void *realloc(void *old, size_t size) {
  if (old == NULL) {
    return malloc(size);
  }

  size_t old_size = *((size_t *)old - 1);

  // Calculate the number of pages needed for the new size
  size_t new_num_pages = (size / PAGE_SIZE) + 1;

  // Calculate the number of pages for the old size
  size_t old_num_pages = (old_size / PAGE_SIZE) + 1;

  // Check if the new size requires more pages than the old size
  if (new_num_pages > old_num_pages) {
    // Request additional pages from the physical memory manager
    void *new_mem = pmm_request_pages(new_num_pages - old_num_pages);
    if (new_mem == NULL) {
      dprintf("[\e[0;31mHeap Error\e[0m] Failed to allocate additional memory "
              "pages\n");
      return NULL; // Memory allocation failed
    }

    // Copy the data from the old block to the new block
    memcpy(new_mem, old, old_size);

    // Free the old block
    free(old);

    return new_mem;
  } else {
    // If the new size fits within the old memory block, just return the old
    // block
    return old;
  }
}
