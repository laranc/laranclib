#include "core/object.h"

void *_i32Clone(Allocator allocator, const void *ptr) {
	const i32 *i = ptr;
	i32 *new_i = new(allocator, i32, 1);
	*new_i = *i;
	return new_i;
}

void _i32Free(Allocator allocator, void *ptr) { delete(allocator, ptr); }

i32 _i32Compare(const void *a, const void *b) {
	i32 x = *(i32 *)a;
	i32 y = *(i32 *)b;
	if (x < y)
		return -1;
	else if (x > y)
		return 1;
	else
		return 0;
}

usize _i32Size(void) { return sizeof(i32); }
