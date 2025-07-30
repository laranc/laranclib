#pragma once

#include <pthread.h>

#include "mem/allocator.h"
#include "types.h"

typedef struct arena_t {
	byte *base;
	usize len;
	usize offset;
	usize used;
	pthread_mutex_t mu;
} Arena;

// Create a new Arena on the heap of len bytes
Arena *arenaNew(usize len);

// Allocate data with a custom alignment
void *arenaAllocAligned(Arena *arena, usize len, usize align);

// Allocate data
void *arenaAlloc(void *ctx, usize size);

// Writes over the base memory with zero
void arenaFreeAll(void *ctx);

// Frees the backing memory and the Arena itself
void arenaDelete(Arena *arena);

// Implement Allocator interface
static inline Allocator arenaAllocatorImpl(Arena *arena) {
	return (Allocator){
		.alloc = arenaAlloc,
		.realloc = NULL,
		.free = NULL,
		.free_all = arenaFreeAll,
		.ctx = arena,
	};
}
