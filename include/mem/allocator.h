#pragma once

#include "types.h"

#ifndef ALLOCATOR_DEFAULT_MEMORY_ALIGNMENT
#define ALLOCATOR_DEFAULT_MEMORY_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct allocator_t {
	void *(*alloc)(void *ctx, usize size);
	void *(*realloc)(void *ctx, usize new_size, void *ptr);
	void (*free)(void *ctx, void *ptr);
	void (*free_all)(void *ctx);
	void *ctx;
} Allocator;

// a: allocator, t: type, n: n elements of t
#define new(a, t, n)	((t *)a.alloc(a.ctx, sizeof(t) * n))

// a: allocator, s: size in bytes
#define make(a, s)		(a.alloc(a.ctx, s))

// a: allocator, s: new size, p: pointer
#define resize(a, s, p) (a.realloc(a.ctx, s, p))

// a: allocator, p: pointer
#define delete(a, p)	(a.free(a.ctx, p))
