#pragma once

#include "mem/allocator.h"

typedef struct object_t {
	void *(*clone)(Allocator allocator, const void *ptr);
	void (*free)(Allocator allocator, void *ptr);
	i32 (*compare)(const void *a, const void *b);
	usize (*size)(void);
} Object;
