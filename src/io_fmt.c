#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>

#include "container/string.h"
#include "io/fmt.h"

void fmtPrint(String string) { write(STDOUT_FILENO, string.base, string.len); }

void fmtPrintln(String string) {
	write(STDOUT_FILENO, string.base, string.len);
	write(STDOUT_FILENO, "\n", 1);
}

String fmtScan(Allocator allocator) {
	StringBuilder builder = stringBuilderNew(allocator, 10);
	char c;
	while (read(STDIN_FILENO, &c, sizeof(char)) > 0) {
		if (isspace(c) || c == '\0')
			break;
		stringBuilderAppendChar(allocator, &builder, c);
	}
	String string = stringBuilderToString(allocator, builder);
	stringBuilderDelete(allocator, builder);
	return string;
}

String fmtScanln(Allocator allocator) {
	StringBuilder builder = stringBuilderNew(allocator, 10);
	char c;
	while (read(STDIN_FILENO, &c, sizeof(char)) > 0) {
		if (c == '\n' || c == '\0')
			break;
		stringBuilderAppendChar(allocator, &builder, c);
	}
	String string = stringBuilderToString(allocator, builder);
	stringBuilderDelete(allocator, builder);
	return string;
}

String _fmtIORead(Allocator allocator, void *) { return fmtScan(allocator); }

Array _fmtIOReadBytes(Allocator allocator, void *) {
	String string = fmtScan(allocator);
	return arrayFromPtr(string.len, sizeof(byte), string.len, string.base);
}

usize _fmtIOWrite(String string, void *) {
	fmtPrint(string);
	return string.len;
}

usize _fmtIOWriteBytes(Array bytes, void *) {
	fmtPrint(stringFromPtr(bytes.base, bytes.used));
	return bytes.used;
}
