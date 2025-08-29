#pragma once

#include "mem/allocator.h"

typedef struct object_t {
	void *(*clone)(Allocator allocator, const void *ptr);
	void (*free)(Allocator allocator, void *ptr);
	i32 (*compare)(const void *a, const void *b);
	usize (*size)(void);
} Object;

// Builtin Objects

void *_i32Clone(Allocator allocator, const void *ptr);
void _i32Free(Allocator allocator, void *ptr);
i32 _i32Compare(const void *a, const void *b);
usize _i32Size(void);
static inline Object i32ObjectImpl(void) {
	return (Object){
		.clone = _i32Clone,
		.free = _i32Free,
		.compare = _i32Compare,
		.size = _i32Size,
	};
}
