#pragma once

#include <stdio.h>

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
Array fileRead(Allocator allocator, File file);

// Reads from the file until it encounters a new line or EOF
String fileReadLine(Allocator allocator, File file);

// Appends the byte Array to the File
usize fileWrite(File file, Array array);

// Reads from the file, returns a pointer, sets number of bytes read
void *filePathReadRaw(Allocator allocator, const char *file_path, usize *bytes);

// Reads from the file, returns an Array of bytes
Array filePathReadBytes(Allocator allocator, const char *file_path);

// Reads from the file, returns a String
String filePathReadString(Allocator allocator, const char *file_path);

// Truncates the file with ptr bytes
usize filePathWriteRaw(const char *file_path, const void *ptr, usize len);

// Truncates the file with Array bytes
usize filePathWriteBytes(const char *file_path, Array array);

// Truncates the file with String chars
usize filePathWriteString(const char *file_path, String string);

// Implement IOReader interface
String _fileIORead(Allocator alloctator, void *ctx);
Array _fileIOReadBytes(Allocator allocator, void *ctx);
static inline IOReader fileIOReaderImpl(File *file) {
	return (IOReader){
		.read = _fileIORead,
		.read_bytes = _fileIOReadBytes,
		.ctx = file,
	};
}

// Implement IOWriter interface
usize _fileIOWrite(String string, void *ctx);
usize _fileIOWriteBytes(Array bytes, void *ctx);
static inline IOWriter fileIOWriterImpl(File *file) {
	return (IOWriter){
		.write = _fileIOWrite,
		.write_bytes = _fileIOWriteBytes,
		.ctx = file,
	};
}
