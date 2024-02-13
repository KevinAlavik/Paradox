#include "pmm.h"

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};
volatile struct limine_memmap_response *memmap;

uint8_t* bitmap; // the actual bitmap list
uint64_t bitmap_pages; // = memorymap_total_free_mem / PAGE_SIZE
uint64_t bitmap_size; // = ALIGN_UP(bitmap_pages / 8, PAGE_SIZE)

void pmm_init() {
    for(int entryCount = 0; entryCount < memmap->entry_count; entryCount++) {
        struct limine_memmap_entry* entry = memmap->entries[entryCount];
        if(entry->type == LIMINE_MEMMAP_USABLE) {
            dprintf("Usable entry at %d\n", entry->base);
        }
    }
}