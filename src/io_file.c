#include "container/string.h"
#include "io/file.h"
#include "mem/allocator.h"

static byte *read(Allocator allocator, FILE *fd, usize *bytes) {
	byte *buf = NULL;
	byte *tmp = NULL;
	usize used = 0;
	usize len = 0;
	usize n = 0;
	for (;;) {
		if (used + FILE_CHUNK_SIZE + 1 > len) {
			len = used + FILE_CHUNK_SIZE + 1;
			if (len <= used) {
				delete(allocator, buf);
				return NULL;
			}
			tmp = resize(allocator, len, buf);
			if (!tmp) {
				delete(allocator, buf);
				return NULL;
			}
			buf = tmp;
		}
		n = fread(buf + used, sizeof(byte), FILE_CHUNK_SIZE, fd);
		if (n == 0)
			break;
		used += n;
	}
	if (ferror(fd)) {
		delete(allocator, buf);
		return NULL;
	}
	*bytes = used;
	return buf;
}

static char *readLine(Allocator allocator, FILE *fd) {
	usize len = STRING_MAX_SIZE;
	char *buf = make(allocator, len);
	char c = getc(fd);
	usize n = 0;
	while (c != '\n' && c != EOF) {
		if (n == STRING_MAX_SIZE) {
			len += STRING_MAX_SIZE;
			buf = resize(allocator, len, buf);
		}
		buf[n] = c;
		n++;
		c = getc(fd);
	}
	buf[n] = '\0';
	buf = resize(allocator, n + 1, buf);
	return buf;
}

File fileOpen(const char *file_path, const char *modes) {
	FILE *fd = fopen(file_path, modes);
	if (ferror(fd))
		return (File){0};
	return (File){
		.fd = fd,
		.is_ok = true,
	};
}

void fileClose(File file) {
	if (file.fd)
		fclose(file.fd);
}

Array fileRead(Allocator allocator, File file) {
	if (!file.is_ok)
		return (Array){0};
	usize len;
	void *data = read(allocator, file.fd, &len);
	return arrayFromPtr(len, sizeof(byte), len, data);
}

String fileReadLine(Allocator allocator, File file) {
	if (!file.is_ok)
		return (String){0};
	return stringFromCStr(readLine(allocator, file.fd));
}

usize fileWrite(File file, Array array) {
	if (!file.is_ok)
		return 0;
	return fwrite(array.base, sizeof(byte), array.len, file.fd);
}

void *filePathReadRaw(Allocator allocator, const char *file_path,
					  usize *bytes) {
	FILE *fd = fopen(file_path, "rb");
	if (ferror(fd))
		return NULL;
	void *buf = read(allocator, fd, bytes);
	fclose(fd);
	return buf;
}

Array filePathReadBytes(Allocator allocator, const char *file_path) {
	usize len;
	byte *data = filePathReadRaw(allocator, file_path, &len);
	if (!data)
		return (Array){0};
	return arrayFromPtr(len, sizeof(byte), len, data);
}

String filePathReadString(Allocator allocator, const char *file_path) {
	usize len;
	char *data = filePathReadRaw(allocator, file_path, &len);
	if (!data)
		return (String){0};
	return stringFromPtr(data, len);
}

usize filePathWriteRaw(const char *file_path, const void *ptr, usize len) {
	FILE *fd = fopen(file_path, "wb");
	if (ferror(fd))
		return 0;
	usize n = fwrite(ptr, sizeof(byte), len, fd);
	fclose(fd);
	return n;
}

usize filePathWriteBytes(const char *file_path, Array array) {
	return filePathWriteRaw(file_path, array.base, array.len);
}

usize filePathWriteString(const char *file_path, String string) {
	return filePathWriteRaw(file_path, string.base, string.len);
}

String _fileIORead(Allocator allocator, void *ctx) {
	File *file = ctx;
	Array bytes = fileRead(allocator, *file);
	return stringFromPtr(bytes.base, bytes.used);
}

Array _fileIOReadBytes(Allocator allocator, void *ctx) {
	File *file = ctx;
	return fileRead(allocator, *file);
}

usize _fileIOWrite(String string, void *ctx) {
	File *file = ctx;
	return fileWrite(
		*file, arrayFromPtr(string.len, sizeof(char), string.len, string.base));
}

usize _fileIOWriteBytes(Array array, void *ctx) {
	File *file = ctx;
	return fileWrite(*file, array);
}
