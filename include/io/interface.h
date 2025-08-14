#pragma once

#include "container/array.h"

typedef struct io_reader_t {
	Array (*read)(Allocator allocator, void *ctx);
	void *ctx;
} IOReader;

typedef struct io_writer_t {
	usize (*write)(Array data, void *ctx);
	void *ctx;
} IOWriter;

// a: Allocator, r: Reader
#define ioread(r, a)  (r.read(a, r.ctx))
// w: Writer, d: Array
#define iowrite(w, d) (w.write(d, w.ctx))
