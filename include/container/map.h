#pragma once

#include <stdbool.h>

#include "mem/allocator.h"
#include "types.h"

#ifndef MAP_INIT_SIZE
#define MAP_INIT_SIZE 10
#endif

#ifndef MAP_COLLISION_FACTOR
#define MAP_COLLISION_FACTOR 0.5
#endif

typedef struct _map_bucket_t {
	void *key;
	usize key_size;
	void *data;
	struct _map_bucket_t *next;
} _MapBucket;

typedef usize (*MapHashFn)(void *key, usize size);
typedef bool (*MapKeyCmp)(void *key1, usize key1_size, void *key2,
						  usize key2_size);

typedef struct map_t {
	_MapBucket **base;
	usize len;
	usize collisions;
	MapHashFn hash_fn;
	MapKeyCmp key_eq;
} Map;

// Allocates the backing data for a Map
Map mapNew(Allocator allocator, MapHashFn hash_fn, MapKeyCmp key_eq);

// Inserts a new element into the Map, without copying.
void mapInsert(Allocator allocator, Map *map, void *key, usize key_size,
			   void *data);

// Fetches an element from the Map, returns NULL if not found
void *mapGet(Map map, void *key, usize size);

// Removes an element from the Map, will not free keys or data
void mapRemove(Allocator allocator, Map *map, void *key, usize size);

// Frees the entire Map, will not free keys or data
void mapDelete(Allocator allocator, Map map);

// Generates a hash from the raw bytes of the key
usize mapDefaultHash(void *key, usize size);

// A default equality function for String keys
bool mapKeyCmpString(void *key1, usize, void *key2, usize);

// A default equality function for Int64 keys
bool mapKeyCmpInt64(void *key1, usize, void *key2, usize);
