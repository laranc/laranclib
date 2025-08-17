#pragma once

#include "container/string.h"

typedef struct io_reader_t {
	String (*read)(Allocator allocator, void *ctx);
	Array (*read_bytes)(Allocator allocator, void *ctx);
	void *ctx;
} IOReader;

typedef struct io_writer_t {
	usize (*write)(String string, void *ctx);
	usize (*write_bytes)(Array bytes, void *ctx);
	void *ctx;
} IOWriter;

// a: Allocator, r: Reader
#define READ(r, a)		  (r.read(a, r.ctx))
// a: Allocator, r: Reader
#define READ_BYTES(r, a)  (r.read_bytes(a, r.ctx))
// w: Writer, d: String
#define WRITE(w, s)		  (w.write(s, w.ctx))
// w: Writer, b: Array bytes
#define WRITE_BYTES(w, b) (w.write(b, w.ctx))
