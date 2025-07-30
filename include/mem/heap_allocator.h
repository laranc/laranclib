#pragma once

#include <stdatomic.h>

#include "mem/allocator.h"
#include "types.h"

typedef struct heap_allocator_t {
	atomic_int alloc_count;
} HeapAllocator;

// Create a new Heap Allocator on the heap
HeapAllocator *heapAllocatorNew(void);

// Call malloc and increment alloc_count
void *heapAlloc(void *ctx, usize size);

// Call realloc
void *heapRealloc(void *, usize new_size, void *ptr);

// Call free and decrement alloc_count
void heapFree(void *ctx, void *ptr);

// Implement Allocator interface
static inline Allocator heapAllocatorImpl(HeapAllocator *heap_allocator) {
	return (Allocator){.alloc = heapAlloc,
					   .realloc = heapRealloc,
					   .free = heapFree,
					   .free_all = NULL,
					   .ctx = heap_allocator};
}
