#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "container/string.h"

String stringNew(Allocator allocator, usize len) {
	return (String){
		.base = make(allocator, len),
		.len = len,
	};
}

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

void stringFree(Allocator allocator, String string) {
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
