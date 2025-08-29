#include <stdlib.h>
#include <string.h>

#include "container/array.h"

Array arrayNew(Allocator allocator, usize cap, usize size) {
	cap = cap > 0 ? cap : ARRAY_INIT_SIZE;
	usize bytes = cap * size;
	void *data = make(allocator, bytes);
	return (Array){
		.base = data,
		.len = 0,
		.cap = cap,
		.size = size,
	};
}

Array arrayFromPtr(usize len, usize size, void *ptr) {
	return (Array){
		.base = ptr,
		.len = len,
		.cap = len,
		.size = size,
	};
}

Array arrayCopy(Allocator allocator, Array source) {
	usize bytes = source.len * source.size;
	void *data = make(allocator, bytes);
	memcpy(data, source.base, bytes);
	return (Array){
		.base = data,
		.len = source.len,
		.cap = source.cap,
		.size = source.size,
	};
}

Array arrayCopyPtr(Allocator allocator, usize len, usize size,
				   const void *ptr) {
	usize bytes = len * size;
	void *data = make(allocator, bytes);
	memcpy(data, ptr, bytes);
	return (Array){
		.base = data,
		.len = len,
		.cap = len,
		.size = size,
	};
}

Array arraySlice(Array source, usize start, usize end) {
	if (start > end || end >= source.len)
		return source;
	return (Array){
		.base = source.base + start,
		.len = source.len > start ? source.len - start : 0,
		.cap = end - start,
		.size = source.size,
	};
}

Array arraySlicePtr(usize start, usize end, usize len, usize size, void *ptr) {
	if (start > end)
		return arrayFromPtr(end - start, size, ptr);
	return (Array){
		.base = ptr + start,
		.len = len > start ? len - start : 0,
		.cap = end - start,
		.size = size,
	};
}

void *arrayGet(Array array, usize idx) {
	if (idx >= array.len)
		return nullptr;
	return array.base + (idx * array.size);
}

void arrayInsert(Array *array, usize idx, const void *data) {
	if (idx >= array->cap)
		return;
	array->len = array->len > idx ? array->len : idx;
	memcpy(array->base + (idx * array->size), data, array->size);
}

void arrayAppend(Allocator allocator, Array *array, const void *data) {
	usize bytes = array->cap * array->size;
	if (array->len < array->cap) {
		array->base = resize(allocator, bytes * 2, array->base);
		array->cap *= 2;
	}
	memcpy(array->base + (array->len * array->size), data, array->size);
	array->len++;
}

Array arrayConcat(Allocator allocator, Array left, Array right) {
	if (left.size != right.size)
		return (Array){0};
	usize len = left.len + right.len;
	usize size = left.size;
	usize bytes = len * size;
	void *data = make(allocator, bytes);
	memcpy(data, left.base, left.len * size);
	memcpy(data + left.len * size, right.base, right.len * size);
	return (Array){
		.base = data,
		.len = len,
		.cap = len,
		.size = size,
	};
}

void arrayRemove(Allocator allocator, Array *array, usize idx) {
	if (idx >= array->len)
		return;
	Array left = arraySlice(*array, 0, idx - 1);
	Array right = arraySlice(*array, idx + 1, array->len - 1);
	arrayConcat(allocator, left, right);
}

void arrayDelete(Allocator allocator, Array array) {
	if (allocator.free)
		delete(allocator, array.base);
}

i32 arrayCmpBytes(Array a, Array b) {
	if (a.base == b.base && a.len == b.len)
		return 0;
	else if (a.len > b.len)
		return 1;
	else if (a.len < b.len)
		return -1;
	const byte *data_a = a.base;
	const byte *data_b = b.base;
	for (usize i = 0; i < a.len; i++) {
		if (data_a[i] > data_b[i])
			return 1;
		else if (data_a[i] < data_b[i])
			return -1;
	}
	return 0;
}

static i32 cmpByte(const void *a, const void *b) {
	byte byte_a = *(byte *)a;
	byte byte_b = *(byte *)b;
	if (byte_a > byte_b)
		return 1;
	else if (byte_a < byte_b)
		return -1;
	else
		return 0;
}

void arrayQsort(Array array, ArrayCmpElem cmp) {
	cmp = cmp ? cmp : cmpByte;
	qsort(array.base, array.len, array.size, cmp);
}

void *_arrayObjClone(Allocator allocator, const void *ptr) {
	Array *array = new(allocator, Array, 1);
	*array = arrayCopy(allocator, *(Array *)ptr);
	return array;
}

void _arrayObjFree(Allocator allocator, void *ptr) {
	arrayDelete(allocator, *(Array *)ptr);
	delete(allocator, ptr);
}

i32 _arrayObjCompare(const void *a, const void *b) {
	const Array *arr_a = a;
	const Array *arr_b = b;
	return arrayCmpBytes(*arr_a, *arr_b);
}

usize _arrayObjSize(void) { return sizeof(Array); }

void *_arrayIterStart(void *ctx, i64 *cnt) {
	Array *array = ctx;
	*cnt = 0;
	return arrayGet(*array, *cnt);
}

void *_arrayIterNext(void *ctx, i64 *cnt) {
	Array *array = ctx;
	return arrayGet(*array, ++(*cnt));
}

void *_arrayIterCurr(void *ctx, i64 *cnt) {
	Array *array = ctx;
	return arrayGet(*array, *cnt);
}

void *_arrayIterPrev(void *ctx, i64 *cnt) {
	Array *array = ctx;
	return arrayGet(*array, --(*cnt));
}

void *_arrayIterEnd(void *ctx, i64 *cnt) {
	Array *array = ctx;
	*cnt = (i64)array->len - 1;
	return arrayGet(*array, *cnt);
}

bool _arrayIterAtStart(void *, i64 *cnt) { return *cnt < 0; }

bool _arrayIterAtEnd(void *ctx, i64 *cnt) {
	Array *array = ctx;
	return *cnt == (i64)array->len;
}
