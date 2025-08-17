#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "container/string.h"

String stringFromCStr(char *str) {
	if (!str)
		return (String){0};
	return (String){
		.base = str,
		.len = strlen(str),
	};
}

String stringFromPtr(char *ptr, usize len) {
	return (String){
		.base = ptr,
		.len = len,
	};
}

char *stringToCStr(Allocator allocator, String string) {
	char *str = make(allocator, string.len + 1);
	memcpy(str, string.base, string.len);
	str[string.len] = '\0';
	return str;
}

String stringCopy(Allocator allocator, String string) {
	char *data = make(allocator, string.len);
	memcpy(data, string.base, string.len);
	return (String){
		.base = data,
		.len = string.len,
	};
}

String stringCopyCStr(Allocator allocator, const char *str) {
	usize len = strlen(str);
	char *data = make(allocator, len);
	memcpy(data, str, len);
	return (String){
		.base = data,
		.len = len,
	};
}

String stringFmt(Allocator allocator, const char *format, ...) {
	va_list args;
	va_start(args, format);
	usize len = vsnprintf(NULL, 0, format, args);
	va_end(args);
	char *data = make(allocator, len);
	va_start(args, format);
	vsnprintf(data, len, format, args);
	va_end(args);
	return (String){
		.base = data,
		.len = len,
	};
}

String stringSlice(String string, usize start, usize end) {
	if (start > end || end > string.len)
		return string;
	return (String){
		.base = string.base + start,
		.len = end - start,
	};
}

String stringConcat(Allocator allocator, String left, String right) {
	usize len = left.len + right.len;
	char *data = make(allocator, len);
	memcpy(data, left.base, left.len);
	memcpy(data + left.len, right.base, right.len);
	return (String){
		.base = data,
		.len = len,
	};
}

void stringDelete(Allocator allocator, String string) {
	if (allocator.free)
		delete(allocator, string.base);
}

void stringToLower(String string) {
	for (usize i = 0; i < string.len; i++) {
		string.base[i] = tolower(string.base[i]);
	}
}

void stringToUpper(String string) {
	for (usize i = 0; i < string.len; i++) {
		string.base[i] = toupper(string.base[i]);
	}
}

Array stringSplit(Allocator allocator, String string, String delims) {
	Array array = ARRAY(allocator, String);
	usize start = 0;
	for (usize i = 0; i < string.len; i++) {
		for (usize j = 0; j < delims.len; j++) {
			if (string.base[i] == delims.base[j]) {
				String slice = stringSlice(string, start, i);
				arrayAppend(allocator, &array, &slice);
				start = i;
				continue;
			}
		}
	}
	return array;
}

i32 stringCmp(String a, String b) {
	if (a.len < b.len) {
		return -1;
	}
	if (b.len < a.len) {
		return 1;
	}
	for (usize i = 0; i < a.len; i++) {
		i32 diff = a.base[i] - b.base[i];
		if (diff != 0)
			return diff;
	}
	return 0;
}

StringBuilder stringBuilderNew(Allocator allocator, usize len) {
	return (StringBuilder){.buffer = arrayNew(allocator, len, sizeof(byte))};
}

void stringBuilderAppendString(Allocator allocator, StringBuilder *builder,
							   String string) {
	Array array =
		arrayFromPtr(string.len, sizeof(byte), string.len, string.base);
	Array new_buf = arrayConcat(allocator, builder->buffer, array);
	arrayDelete(allocator, builder->buffer);
	builder->buffer = new_buf;
}

void stringBuilderAppendBytes(Allocator allocator, StringBuilder *builder,
							  Array bytes) {
	Array new_buf = arrayConcat(allocator, builder->buffer, bytes);
	arrayDelete(allocator, builder->buffer);
	builder->buffer = new_buf;
}

void stringBuilderAppendChar(Allocator allocator, StringBuilder *builder,
							 char c) {
	arrayAppend(allocator, &builder->buffer, &c);
}

String stringBuilderToString(Allocator allocator, StringBuilder builder) {
	return stringCopy(allocator,
					  stringFromPtr(builder.buffer.base, builder.buffer.used));
}

void stringBuilderDelete(Allocator allocator, StringBuilder builder) {
	arrayDelete(allocator, builder.buffer);
}

void *_stringObjClone(Allocator allocator, const void *ptr) {
	String *string = new(allocator, String, 1);
	*string = stringCopy(allocator, *(String *)ptr);
	return string;
}

void _stringObjFree(Allocator allocator, void *ptr) {
	stringDelete(allocator, *(String *)ptr);
	delete(allocator, ptr);
}

i32 _stringObjCompare(const void *a, const void *b) {
	return stringCmp(*(String *)a, *(String *)b);
}

usize _stringObjSize(void) { return sizeof(String); }
