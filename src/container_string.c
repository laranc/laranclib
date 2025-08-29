#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "container/array.h"
#include "container/string.h"
#include "core/iterator.h"

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

static bool isDelim(char c, String delims) {
	for (usize i = 0; i < delims.len; i++) {
		if (c == delims.base[i])
			return true;
	}
	return false;
}

Array stringSplit(Allocator allocator, String string, String delims) {
	usize count = 0;
	for (usize i = 0; i < string.len; i++) {
		if (isDelim(string.base[i], delims))
			count++;
	}
	Array array = arrayNew(allocator, count + 1, sizeof(String));
	usize start = 0;
	usize idx = 0;
	for (usize i = 0; i < string.len; i++) {
		if (!isDelim(string.base[i], delims))
			continue;
		String substr = stringSlice(string, start, i);
		arrayInsert(&array, idx, &substr);
		start = i + 1;
		idx++;
	}
	String substr = stringSlice(string, start, string.len);
	arrayInsert(&array, idx, &substr);
	return array;
}

i32 stringCmp(String a, String b) {
	if (a.len < b.len) {
		return -1;
	} else if (b.len < a.len) {
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

void stringBuilderAppendChar(Allocator allocator, StringBuilder *builder,
							 char c) {
	arrayAppend(allocator, &builder->buffer, &c);
}

void stringBuilderAppendString(Allocator allocator, StringBuilder *builder,
							   String string) {
	Iterator iter = stringIter(&string);
	for (char *c = ITER_START(iter); !ITER_AT_END(iter); c = ITER_NEXT(iter))
		arrayAppend(allocator, &builder->buffer, c);
}

void stringBuilderAppendBytes(Allocator allocator, StringBuilder *builder,
							  Array bytes) {
	Iterator iter = arrayIter(&bytes);
	for (byte *b = ITER_START(iter); !ITER_AT_END(iter); b = ITER_NEXT(iter))
		arrayAppend(allocator, &builder->buffer, b);
}

String stringBuilderToString(StringBuilder builder) {
	return stringFromPtr(builder.buffer.base, builder.buffer.len);
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

void *_stringIterStart(void *ctx, i64 *cnt) {
	String *string = ctx;
	*cnt = 0;
	return string->base;
}

void *_stringIterNext(void *ctx, i64 *cnt) {
	String *string = ctx;
	return &string->base[++(*cnt)];
}

void *_stringIterCurr(void *ctx, i64 *cnt) {
	String *string = ctx;
	return &string->base[*cnt];
}

void *_stringIterPrev(void *ctx, i64 *cnt) {
	String *string = ctx;
	return &string->base[--(*cnt)];
}

void *_stringIterEnd(void *ctx, i64 *cnt) {
	String *string = ctx;
	*cnt = (i64)string->len - 1;
	return &string->base[*cnt];
}

bool _stringIterAtStart(void *, i64 *cnt) { return *cnt < 0; }

bool _stringIterAtEnd(void *ctx, i64 *cnt) {
	String *string = ctx;
	return *cnt == (i64)string->len;
}
