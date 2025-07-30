#include <string.h>

#include "container/array.h"

Array arrayNew(Allocator allocator, usize len, usize size) {
	usize bytes = len * size;
	void *data = make(allocator, bytes);
	memset(data, 0, bytes);
	return (Array){
		.base = data,
		.len = len,
		.size = size,
		.used = 0,
	};
}

Array arrayFromPtr(usize len, usize size, usize used, void *ptr) {
	return (Array){
		.base = ptr,
		.len = len,
		.size = size,
		.used = used,
	};
}

Array arrayCopy(Allocator allocator, Array source) {
	usize bytes = source.len * source.size;
	void *data = make(allocator, bytes);
	memcpy(data, source.base, bytes);
	return (Array){
		.base = data,
		.len = source.len,
		.size = source.size,
		.used = source.used,
	};
}

Array arrayCopyPtr(Allocator allocator, usize len, usize size, usize used,
				   const void *ptr) {
	usize bytes = len * size;
	void *data = make(allocator, bytes);
	memcpy(data, ptr, bytes);
	return (Array){
		.base = data,
		.len = len,
		.size = size,
		.used = used,
	};
}

Array arraySlice(Array source, usize start, usize end) {
	if (start > end || end >= source.len)
		return source;
	return (Array){
		.base = source.base + start,
		.len = end - start,
		.size = source.size,
		.used = source.used > start ? source.used - start : 0,
	};
}

Array arraySlicePtr(usize start, usize end, usize used, usize size, void *ptr) {
	if (start > end)
		return arrayFromPtr(end - start, size, used, ptr);
	return (Array){
		.base = ptr + start,
		.len = end - start,
		.size = size,
		.used = used > start ? used - start : 0,
	};
}

void *arrayGet(Array array, usize idx) {
	if (idx >= array.len)
		return NULL;
	return array.base + (idx * array.size);
}

void arrayInsert(Array array, usize idx, void *data) {
	if (idx >= array.len)
		return;
	memcpy(array.base + (idx * array.size), data, array.size);
}

Array arrayAppend(Allocator allocator, Array array, void *data) {
	usize bytes = array.len * array.size;
	if (array.used < array.len) {
		void *new_base = make(allocator, bytes * 2);
		memcpy(new_base, array.base, array.len * array.size);
		array.base = new_base;
		array.len *= 2;
	}
	memcpy(array.base + (array.used * array.size), data, array.size);
	array.used++;
	return array;
}

Array arrayConcat(Allocator allocator, Array left, Array right) {
	if (left.size != right.size)
		return (Array){0};
	usize len = left.used + right.used;
	usize size = left.size;
	usize bytes = len * size;
	void *data = make(allocator, bytes);
	memcpy(data, left.base, left.used * size);
	memcpy(data + left.used * size, right.base, right.used * size);
	return (Array){
		.base = data,
		.len = len,
		.size = size,
		.used = len,
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
