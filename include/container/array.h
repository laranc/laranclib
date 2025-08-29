#pragma once

#include "core/iterator.h"
#include "core/object.h"

#ifndef ARRAY_INIT_SIZE
#define ARRAY_INIT_SIZE 10
#endif

typedef i32 (*ArrayCmpElem)(const void *a, const void *b);

typedef struct array_t {
	void *base;
	usize len;
	usize cap;
	usize size;
} Array;

// Shortcut macros
// a: Allocator, t: type
#define ARRAY(a, t)		   (arrayNew(a, 0, sizeof(t)))
// a: Array, t: type, i: index
#define ARRAY_GET(a, t, i) (*(t *)arrayGet(a, i))

// Allocates a new array of size: len (n elements) * size (bytes per element)
Array arrayNew(Allocator allocator, usize cap, usize size);

// Creates a new array with ptr as the backing buffer
Array arrayFromPtr(usize len, usize size, void *ptr);

// Creates a copy of an array
Array arrayCopy(Allocator allocator, Array source);

// Creates a copy from a raw pointer
Array arrayCopyPtr(Allocator allocator, usize len, usize size, const void *ptr);

// Creates a new array without copying the backing data
Array arraySlice(Array source, usize start, usize end);

// Creates a new array without copying the pointer
Array arraySlicePtr(usize start, usize end, usize len, usize size, void *ptr);

// Gets the element at the supplied index
void *arrayGet(Array array, usize idx);

// Inserts an element at the supplied index
void arrayInsert(Array *array, usize idx, const void *data);

// Appends a new element the array, if the array has no room a new buffer will
// be allocated
void arrayAppend(Allocator allocator, Array *array, const void *data);

// Allocates a new array from the elements of the left and right arrays
Array arrayConcat(Allocator allocator, Array left, Array right);

// Removes an element from the array and returns the altered array
void arrayRemove(Allocator allocator, Array *array, usize idx);

// Frees the backing buffer
void arrayDelete(Allocator allocator, Array array);

// Compares two arrays by their bytes
i32 arrayCmpBytes(Array a, Array b);

// Sort the array using the comparison function
void arrayQsort(Array array, ArrayCmpElem cmp);

// Implement Object interface
void *_arrayObjClone(Allocator allocator, const void *ptr);
void _arrayObjFree(Allocator allocator, void *ptr);
i32 _arrayObjCompare(const void *a, const void *b);
usize _arrayObjSize(void);
static inline Object arrayObject(void) {
	return (Object){
		.clone = _arrayObjClone,
		.free = _arrayObjFree,
		.compare = _arrayObjCompare,
		.size = _arrayObjSize,
	};
}

// Implement Iterator interface
void *_arrayIterStart(void *ctx, i64 *cnt);
void *_arrayIterNext(void *ctx, i64 *cnt);
void *_arrayIterCurr(void *ctx, i64 *cnt);
void *_arrayIterPrev(void *ctx, i64 *cnt);
void *_arrayIterEnd(void *ctx, i64 *cnt);
bool _arrayIterAtStart(void *ctx, i64 *cnt);
bool _arrayIterAtEnd(void *ctx, i64 *cnt);
static inline Iterator arrayIter(Array *array) {
	return (Iterator){
		.start = _arrayIterStart,
		.next = _arrayIterNext,
		.curr = _arrayIterCurr,
		.prev = _arrayIterPrev,
		.end = _arrayIterEnd,
		.at_start = _arrayIterAtStart,
		.at_end = _arrayIterAtEnd,
		.ctx = array,
		.cnt = 0,
	};
}
