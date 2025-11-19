#ifndef MM_H
#define MM_H

#include <stddef.h>
#include <stdint.h>

/*
 * Initialize the kernel heap.
 *  - heap_start: address where the heap begins
 *  - heap_size : size of the heap in bytes
 */
void kheap_init(void *heap_start, size_t heap_size);

/*
 * Allocate 'size' bytes from the heap.
 * Returns NULL if out of memory.
 */
void *kmalloc(size_t size);

/*
 * Allocate 'size' bytes with a specific alignment.
 * 'alignment' should be a power of 2 (4, 8, 16, 4096, ...).
 */
void *kmalloc_aligned(size_t size, size_t alignment);

#endif

