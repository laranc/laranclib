#pragma once

#include <pthread.h>

#include "mem/allocator.h"

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
void *arenaAlloc(Arena *arena, usize size);

// Writes over the base memory with zero
void arenaFreeAll(Arena *arena);

// Frees the backing memory and the Arena itself
void arenaDelete(Arena *arena);

// Implement Allocator interface
void *_arenaAlloc(usize len, void *ctx);
void *_arenaRealloc(usize new_size, void *ptr, void *ctx);
void _arenaFree(void *ptr, void *ctx);
void _arenaFreeAll(void *ctx);
static inline Allocator arenaAllocatorImpl(Arena *arena) {
	return (Allocator){
		.alloc = _arenaAlloc,
		.realloc = _arenaRealloc,
		.free = _arenaFree,
		.free_all = _arenaFreeAll,
		.ctx = arena,
	};
}
