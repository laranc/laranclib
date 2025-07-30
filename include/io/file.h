#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "container/array.h"
#include "container/string.h"
#include "io/interface.h"

#ifndef FILE_CHUNK_SIZE
#define FILE_CHUNK_SIZE 2097152
#endif

typedef struct file_t {
	FILE *fd;
	bool is_ok;
} File;

// Creates a new file descriptor
File fileOpen(const char *file_path, const char *modes);

// Closes the file descriptor
void fileClose(File file);

// Reads from the File, returning a byte Array
Array fileRead(void *ctx, Allocator allocator);

// Appends the byte Array to the File
usize fileWrite(void *ctx, Array array);

// Reads from the file, returns a pointer, sets number of bytes read
void *filePathReadRaw(Allocator allocator, const char *file_path, usize *bytes);

// Reads from the file, returns an Array of bytes
Array filePathReadBytes(Allocator allocator, const char *file_path);

// Reads from the file, returns a String
String filePathReadString(Allocator allocator, const char *file_path);

// Truncates the file with ptr bytes
usize filePathWriteRaw(const char *file_path, void *ptr, usize len);

// Truncates the file with Array bytes
usize filePathWriteBytes(const char *file_path, Array array);

// Truncates the file with String chars
usize filePathWriteString(const char *file_path, String string);

// Implement IOReader interface
static inline IOReader fileIOReaderImpl(File *file) {
	return (IOReader){.read = fileRead, .ctx = file};
}

// Implement IOWriter interface
static inline IOWriter fileIOWriterImpl(File *file) {
	return (IOWriter){.write = fileWrite, .ctx = file};
}
