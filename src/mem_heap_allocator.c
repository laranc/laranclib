#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#include "mem/heap_allocator.h"

HeapAllocator *heapAllocatorNew(void) {
	HeapAllocator *heap_allocator = malloc(sizeof(HeapAllocator));
	atomic_init(&heap_allocator->alloc_count, 0);
	return heap_allocator;
}

void *heapAlloc(HeapAllocator *heap_allocator, usize size) {
	atomic_fetch_add(&heap_allocator->alloc_count, 1);
	void *ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void *heapRealloc(HeapAllocator *heap_allocator, usize new_size, void *ptr) {
	if (!ptr)
		atomic_fetch_add(&heap_allocator->alloc_count, 1);
	return realloc(ptr, new_size);
}

void heapFree(HeapAllocator *heap_allocator, void *ptr) {
	atomic_fetch_sub(&heap_allocator->alloc_count, 1);
	free(ptr);
}

void *_heapAlloc(usize size, void *ctx) {
	HeapAllocator *heap_allocator = ctx;
	return heapAlloc(heap_allocator, size);
}

void *_heapRealloc(usize new_size, void *ptr, void *ctx) {
	HeapAllocator *heap_allocator = ctx;
	return heapRealloc(heap_allocator, new_size, ptr);
}

void _heapFree(void *ptr, void *ctx) {
	HeapAllocator *heap_allocator = ctx;
	heapFree(heap_allocator, ptr);
}

void _heapFreeAll(void *) {
	assert(0 && "HeapAllocator does not support Allocator.free_all");
}
