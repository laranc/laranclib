#pragma once

#include "container/array.h"

#ifndef STRING_MAX_SIZE
#define STRING_MAX_SIZE 10000
#endif

typedef struct string_t {
	char *base;
	usize len;
} String;

typedef struct string_builder_t {
	Array buffer;
} StringBuilder;

// Shorthand constructor
#define STRING(cstr) (stringFromCStr(cstr))

// Create a String from a null terminated string without copying
String stringFromCStr(char *str);

// Create a String from a non-null terminated char pointer
String stringFromPtr(char *ptr, usize len);

// Creates a null terminated string from a String
char *stringToCStr(Allocator allocator, String string);

// Return a new String from the source String
String stringCopy(Allocator allocator, String string);

// Return a new String from a null terminated string
String stringCopyCStr(Allocator allocator, const char *str);

// Creates a new String from a format string
String stringFmt(Allocator allocator, const char *format, ...);

// Returns a a new String slice from the data of the original String
String stringSlice(String string, usize start, usize end);

// Creates a new String by joining the left and right Strings
String stringConcat(Allocator allocator, String left, String right);

// Free a the backing data of a String
void stringDelete(Allocator allocator, String string);

// Modifies the underlying data to set all chars to lower case
void stringToLower(String string);

// Modifies the underlying data to set all chars to upper case
void stringToUpper(String string);

// Returns an array of Strings, split by the delimiters
Array stringSplit(Allocator allocator, String string, String delims);

// Returns the lexical difference between a and b
i32 stringCmp(String a, String b);

// Allocates a new StringBuilder
StringBuilder stringBuilderNew(Allocator allocator, usize len);

// Add a String to the internal buffer
void stringBuilderAppendString(Allocator allocator, StringBuilder *builder,
							   String string);

// Add a byte Array to the internal buffer
void stringBuilderAppendBytes(Allocator allocator, StringBuilder *builder,
							  Array bytes);

// Add a byte to the internal buffer
void stringBuilderAppendChar(Allocator allocator, StringBuilder *builder,
							 char c);

// Allocates a new String from the internal buffer
String stringBuilderToString(Allocator allocator, StringBuilder builder);

// Frees the internal buffer
void stringBuilderDelete(Allocator allocator, StringBuilder builder);

// Implement Object interface
void *_stringObjClone(Allocator allocator, const void *ptr);
void _stringObjFree(Allocator allocator, void *ptr);
i32 _stringObjCompare(const void *a, const void *b);
usize _stringObjSize(void);
static inline Object stringObject(void) {
	return (Object){
		.clone = _stringObjClone,
		.free = _stringObjFree,
		.compare = _stringObjCompare,
		.size = _stringObjSize,
	};
}
