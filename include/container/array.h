#pragma once

#include "core/object.h"
#include "mem/allocator.h"

#ifndef ARRAY_INIT_SIZE
#define ARRAY_INIT_SIZE 10
#endif

typedef i32 (*ArrayCmpElem)(const void *a, const void *b);

typedef struct array_t {
	void *base;
	usize len;
	usize size;
	usize used;
} Array;

// Allocates a new array of size: len (n elements) * size (bytes per element)
Array arrayNew(Allocator allocator, usize len, usize size);

// Creates a new array with ptr as the backing buffer
Array arrayFromPtr(usize len, usize size, usize used, void *ptr);

// Creates a copy of an array
Array arrayCopy(Allocator allocator, Array source);

// Creates a copy from a raw pointer
Array arrayCopyPtr(Allocator allocator, usize len, usize size, usize used,
				   const void *ptr);

// Creates a new array without copying the backing data
Array arraySlice(Array source, usize start, usize end);

// Creates a new array without copying the pointer
Array arraySlicePtr(usize start, usize end, usize used, usize size, void *ptr);

// Gets the element at the supplied index
void *arrayGet(Array array, usize idx);

// Inserts an element at the supplied index
void arrayInsert(Array array, usize idx, void *data);

// Appends a new element the array, if the array has no room a new buffer will
// be allocated
Array arrayAppend(Allocator allocator, Array array, void *data);

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
