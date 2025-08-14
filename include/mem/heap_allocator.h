#pragma once

#include <stdatomic.h>

#include "mem/allocator.h"

typedef struct heap_allocator_t {
	atomic_int alloc_count;
} HeapAllocator;

// Create a new Heap Allocator on the heap
HeapAllocator *heapAllocatorNew(void);

// Call malloc and increment alloc_count
void *heapAlloc(HeapAllocator *heap_allocator, usize size);

// Call realloc
void *heapRealloc(HeapAllocator *heap_allocator, usize new_size, void *ptr);

// Call free and decrement alloc_count
void heapFree(HeapAllocator *heap_allocator, void *ptr);

// Implement Allocator interface
void *_heapAlloc(usize size, void *ctx);
void *_heapRealloc(usize new_size, void *ptr, void *ctx);
void _heapFree(void *ptr, void *ctx);
void _heapFreeAll(void *ctx);
static inline Allocator heapAllocatorImpl(HeapAllocator *heap_allocator) {
	return (Allocator){
		.alloc = _heapAlloc,
		.realloc = _heapRealloc,
		.free = _heapFree,
		.free_all = _heapFreeAll,
		.ctx = heap_allocator,
	};
}
