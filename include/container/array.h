#pragma once

#include "mem/allocator.h"
#include "types.h"

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
