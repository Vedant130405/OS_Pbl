#include "./include/mm.h"

static uint8_t *heap_base   = 0;
static size_t   heap_size   = 0;
static size_t   heap_offset = 0;

/*
 * Initialize the simple bump-allocator heap.
 */
void kheap_init(void *heap_start, size_t size)
{
    heap_base   = (uint8_t *)heap_start;
    heap_size   = size;
    heap_offset = 0;
}

/*
 * Align 'value' up to the next multiple of 'alignment'.
 */
static size_t align_up(size_t value, size_t alignment)
{
    if (alignment == 0)
        return value;

    size_t remainder = value % alignment;
    if (remainder == 0)
        return value;

    return value + (alignment - remainder);
}

/*
 * Allocate memory with a given alignment.
 */
void *kmalloc_aligned(size_t size, size_t alignment)
{
    if (!heap_base || size == 0)
        return 0;

    // Current pointer = heap_base + heap_offset
    size_t current_addr = (size_t)(heap_base + heap_offset);

    // Align that address
    size_t aligned_addr = align_up(current_addr, alignment);
    size_t diff         = aligned_addr - current_addr;

    // Check if there is enough space
    if (heap_offset + diff + size > heap_size)
        return 0; // Out of memory

    // Move offset to aligned point
    heap_offset += diff;

    // Pointer to the allocated block
    void *ptr = heap_base + heap_offset;

    // Advance offset by requested size
    heap_offset += size;

    return ptr;
}

/*
 * Allocate memory with default 8-byte alignment.
 */
void *kmalloc(size_t size)
{
    return kmalloc_aligned(size, 8);
}

