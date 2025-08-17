#pragma once

#include "container/string.h"
#include "io/interface.h"

// Print String to stdout
void fmtPrint(String string);

// Print String and newline char to stdout
void fmtPrintln(String string);

// Read from stdin until encountering whitespace
String fmtScan(Allocator allocator);

// Read from stdin until encountering  a new line
String fmtScanln(Allocator allocator);

// Implement IOReader interface
String _fmtIORead(Allocator allocator, void *ctx);
Array _fmtIOReadBytes(Allocator allocator, void *ctx);
static inline IOReader fmtReader(void) {
	return (IOReader){
		.read = _fmtIORead,
		.read_bytes = _fmtIOReadBytes,
		.ctx = NULL,
	};
}

// Implement IOWriter interface
usize _fmtIOWrite(String string, void *ctx);
usize _fmtIOWriteBytes(Array bytes, void *ctx);
static inline IOWriter fmtWriter(void) {
	return (IOWriter){
		.write = _fmtIOWrite,
		.write_bytes = _fmtIOWriteBytes,
		.ctx = NULL,
	};
}
