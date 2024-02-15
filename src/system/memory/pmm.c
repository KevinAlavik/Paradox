#include "pmm.h"

#include <system/bitmap/bitmap.h>

#include <kernel/boot.h>
#include <nighterm/nighterm.h>
#include <nighterm/backends/vga.h>
#include <math.h>

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};
volatile struct limine_memmap_response *memmap;

uint8_t *bitmap;
uint64_t bitmap_pages;
uint64_t bitmap_size;
uint64_t free_memory;

void init_pmm()
{
    memmap = memmap_request.response;
    uint64_t top_address;
    uint64_t higher_address = 0;
    uint64_t hhdm_offset = hhdm_request.response->offset;

    for (int entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            top_address = entry->base + entry->length;
            if (top_address > higher_address)
                higher_address = top_address;

            dprintf("[Physical Memory Manager] Usable entry at 0x%016llX, Top Address: 0x%016llX, Higher Address: 0x%016llX\n", entry->base, top_address, higher_address);
        }
    }
    bitmap_pages = higher_address / PAGE_SIZE;
    dprintf("[Physical Memory Manager] - Bitmap Pages: 0x%016llX\n", bitmap_pages);
    bitmap_size = ALIGN_UP(bitmap_pages / 8, PAGE_SIZE);
    dprintf("[Physical Memory Manager] - Bitmap Size: 0x%016llX\n", bitmap_size);

    update_memory();

    for (int entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            if (entry->length >= bitmap_size)
            {
                dprintf("[Physical Memory Manager] Setting bitmap pointer to 0x%016llX\n", entry->base);
                bitmap = entry->base + hhdm_offset;
                dprintf("[Physical Memory Manager] Set bitmap pointer to 0x%016llX\n", bitmap);
                memset(bitmap, 0xFF, bitmap_size);
                entry->base += bitmap_size;
                dprintf("[Physical Memory Manager] Encreased entry->base by bitmap_size (0x%016llX)\n", entry->base);
                entry->length -= bitmap_size;
                dprintf("[Physical Memory Manager] Decreased entry->length by bitmap_size (0x%016llX)\n", entry->length);
                break;
            }
        }
    }

    for (int entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            for (uint64_t i = 0; i < entry->length; i += PAGE_SIZE)
            {
                bitmap_clear(bitmap, (entry->base + i) / PAGE_SIZE);
            }
        }
    }
}

void update_memory()
{
    free_memory = 0;
    for (int entryCount = 0; entryCount < memmap->entry_count; entryCount++)
    {
        struct limine_memmap_entry *entry = memmap->entries[entryCount];

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            free_memory += entry->length;
        }
    }
}

void *pmm_request_page()
{
    uint64_t last_bit_val;
    uint64_t last_allocated_index;

    while (1)
    {
        last_bit_val = bitmap_get(bitmap, last_allocated_index);
        if (last_bit_val == 0)
        {
            return (void *)(last_allocated_index * PAGE_SIZE);
        }
        else
        {
            if (last_allocated_index > bitmap_pages)
            {
                return NULL;
            }
            else
            {
                last_allocated_index++;
            }
        }
    }
}

void pmm_free(void *ptr)
{
    uint64_t bit_idx = ((uint64_t)ptr / PAGE_SIZE);
    bitmap_clear(bitmap, bit_idx);
}

void visualize_pmm(int startX, int startY, int width, int height)
{
    update_memory();
    if (bitmap == NULL || framebuffer == NULL)
    {
        return;
    }

    // Ensure the startY and startX values are within bounds
    if (startY < 0)
        startY = 0;
    if (startX < 0)
        startX = 0;

    // Ensure the height and width don't exceed the framebuffer dimensions
    if (startY + height > framebuffer->height)
        height = framebuffer->height - startY;
    if (startX + width > framebuffer->width)
        width = framebuffer->width - startX;

    for (int y = startY; y < startY + height; y++)
    {
        for (int x = startX; x < startX + width; x++)
        {
            int i = y * framebuffer->width + x;
            uint8_t value = (bitmap[i / 8] >> (i % 8)) & 1;
            if (value)
            {
                // Allocated memory - Red
                nighterm_putpixel(x, y, 255, 0, 0);
            }
            else
            {
                // Free memory - Green
                nighterm_putpixel(x, y, 0, 255, 0);
            }
        }
    }
}

// MEMORY FUNCTIONS

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++)
    {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++)
    {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest)
    {
        for (size_t i = 0; i < n; i++)
        {
            pdest[i] = psrc[i];
        }
    }
    else if (src < dest)
    {
        for (size_t i = n; i > 0; i--)
        {
            pdest[i - 1] = psrc[i - 1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++)
    {
        if (p1[i] != p2[i])
        {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}
