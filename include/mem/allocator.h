#pragma once

#include "core/types.h"

#ifndef ALLOCATOR_DEFAULT_MEMORY_ALIGNMENT
#define ALLOCATOR_DEFAULT_MEMORY_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct allocator_t {
	void *(*alloc)(usize size, void *ctx);
	void *(*realloc)(usize new_size, void *ptr, void *ctx);
	void (*free)(void *ptr, void *ctx);
	void (*free_all)(void *ctx);
	void *ctx;
} Allocator;

// a: Allocator, t: type, n: n elements of t
#define new(a, t, n)	((t *)a.alloc(sizeof(t) * n, a.ctx))
// a: Allocator, s: size in bytes
#define make(a, s)		(a.alloc(s, a.ctx))
// a: Allocator, s: new size, p: pointer
#define resize(a, s, p) (a.realloc(s, p, a.ctx))
// a: Allocator, p: pointer
#define delete(a, p)	(a.free(p, a.ctx))
#define clear(a)		(a.free_all(a.ctx))
