#include <stdatomic.h>
#include <stdlib.h>

#include "mem/heap_allocator.h"

HeapAllocator *heapAllocatorNew(void) {
	HeapAllocator *heap_allocator = malloc(sizeof(HeapAllocator));
	atomic_init(&heap_allocator->alloc_count, 0);
	return heap_allocator;
}

void *heapAlloc(void *ctx, usize size) {
	HeapAllocator *heap_allocator = ctx;
	atomic_fetch_add(&heap_allocator->alloc_count, 1);
	return malloc(size);
}

void *heapRealloc(void *, usize new_size, void *ptr) {
	return realloc(ptr, new_size);
}

void heapFree(void *ctx, void *ptr) {
	HeapAllocator *heap_allocator = ctx;
	atomic_fetch_sub(&heap_allocator->alloc_count, 1);
	free(ptr);
}
