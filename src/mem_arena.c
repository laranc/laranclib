#include <stdlib.h>
#include <string.h>

#include "mem/allocator.h"
#include "mem/arena.h"

Arena *arenaNew(usize len) {
	Arena *arena = malloc(sizeof(Arena));
	*arena = (Arena){
		.base = calloc(1, len),
		.len = len,
		.offset = 0,
		.used = 0,
	};
	pthread_mutex_init(&arena->mu, NULL);
	return arena;
}

#define isPowerOfTwo(x) ((x != 0) && ((x & (x - 1)) == 0))

static uptr alignForward(uptr ptr, usize align) {
	if (!isPowerOfTwo(align))
		return 0;
	uptr p = ptr;
	uptr a = (uptr)align;
	uptr m = p & (a - 1);
	if (m) {
		p += a - m;
	}
	return p;
}

void *arenaAllocAligned(Arena *arena, usize size, usize align) {
	pthread_mutex_lock(&arena->mu);
	uptr curr = (uptr)arena->base + (uptr)arena->offset;
	uptr offset = alignForward(curr, align);
	offset -= (uptr)arena->base;
	if (offset + size > arena->len) {
		return NULL;
	}
	arena->used += size;
	void *ptr = arena->base + offset;
	arena->offset = offset + size;
	pthread_mutex_unlock(&arena->mu);
	return ptr;
}

void *arenaAlloc(void *ctx, usize size) {
	Arena *arena = ctx;
	if (size == 0) {
		return NULL;
	}
	return arenaAllocAligned(arena, size, ALLOCATOR_DEFAULT_MEMORY_ALIGNMENT);
}

void arenaFreeAll(void *ctx) {
	Arena *arena = ctx;
	pthread_mutex_lock(&arena->mu);
	memset(arena->base, 0, arena->len);
	arena->offset = 0;
	arena->used = 0;
	pthread_mutex_unlock(&arena->mu);
}

void arenaDelete(Arena *arena) {
	free(arena->base);
	free(arena);
}
