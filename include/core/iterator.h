#pragma once

#include "core/types.h"

typedef struct iterator_t {
	void *(*start)(void *ctx, i64 *cnt);
	void *(*next)(void *ctx, i64 *cnt);
	void *(*curr)(void *ctx, i64 *cnt);
	void *(*prev)(void *ctx, i64 *cnt);
	void *(*end)(void *ctx, i64 *cnt);
	bool (*at_start)(void *ctx, i64 *cnt);
	bool (*at_end)(void *ctx, i64 *cnt);
	void *ctx;
	i64 cnt;
} Iterator;

#define ITER_START(i)	 (i.start(i.ctx, &i.cnt))
#define ITER_NEXT(i)	 (i.next(i.ctx, &i.cnt))
#define ITER_CURR(i)	 (i.curr(i.ctx, &i.cnt))
#define ITER_PREV(i)	 (i.prev(i.ctx, &i.cnt))
#define ITER_END(i)		 (i.end(i.ctx, &i.cnt))
#define ITER_AT_START(i) (i.at_start(i.ctx, &i.cnt))
#define ITER_AT_END(i)	 (i.at_end(i.ctx, &i.cnt))
