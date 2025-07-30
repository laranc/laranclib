#pragma once

#include "container/array.h"

typedef struct io_reader_t {
	Array (*read)(void *ctx, Allocator allocator);
	void *ctx;
} IOReader;

typedef struct io_writer_t {
	usize (*write)(void *ctx, Array data);
	void *ctx;
} IOWriter;

// r: reader, a: allocator
#define ioread(r, a)  (r.read(r.ctx, a))

// w: writer, a: allocator
#define iowrite(w, d) (w.write(w.ctx, d))