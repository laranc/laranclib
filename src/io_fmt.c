#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include "container/string.h"
#include "io/fmt.h"

void fmtPrint(String string) { write(STDOUT_FILENO, string.base, string.len); }

void fmtPrintln(String string) {
	write(STDOUT_FILENO, string.base, string.len);
	write(STDOUT_FILENO, "\n", 1);
}

void fmtPrintf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	usize len = vsnprintf(NULL, 0, format, args) + 1;
	va_end(args);
	char data[len];
	va_start(args, format);
	vsnprintf(data, len, format, args);
	va_end(args);
	write(STDOUT_FILENO, data, len);
}

String fmtScan(Allocator allocator) {
	StringBuilder builder = stringBuilderNew(allocator, 10);
	char c;
	while (read(STDIN_FILENO, &c, sizeof(char)) > 0) {
		if (isspace(c) || c == '\0')
			break;
		stringBuilderAppendChar(allocator, &builder, c);
	}
	return stringBuilderToString(builder);
}

String fmtScanln(Allocator allocator) {
	StringBuilder builder = stringBuilderNew(allocator, 10);
	char c;
	while (read(STDIN_FILENO, &c, sizeof(char)) > 0) {
		if (c == '\n' || c == '\0')
			break;
		stringBuilderAppendChar(allocator, &builder, c);
	}
	return stringBuilderToString(builder);
}

void fmtScanf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vscanf(format, args);
	va_end(args);
}

String _fmtIORead(Allocator allocator, void *) { return fmtScan(allocator); }

Array _fmtIOReadBytes(Allocator allocator, void *) {
	String string = fmtScan(allocator);
	return arrayFromPtr(string.len, sizeof(byte), string.base);
}

usize _fmtIOWrite(String string, void *) {
	fmtPrint(string);
	return string.len;
}

usize _fmtIOWriteBytes(Array bytes, void *) {
	fmtPrint(stringFromPtr(bytes.base, bytes.len));
	return bytes.len;
}
