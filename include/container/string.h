#pragma once

#include "mem/allocator.h"
#include "types.h"

typedef struct string_t {
	char *base;
	usize len;
} String;

// Allocate a new String
String stringNew(Allocator allocator, usize len);

// Create a String from a null terminated string without copying
String stringFromCStr(char *str);

// Create a string from a non-null terminated char pointer
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
void stringFree(Allocator allocator, String string);

// Modifies the underlying data to set all chars to lower case
void stringToLower(String string);

// Modifies the underlying data to set all chars to upper case
void stringToUpper(String string);

// Returns the lexical difference between a and b
i32 stringCmp(String a, String b);
