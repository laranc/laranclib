#pragma once

#include "core/object.h"
#include "mem/allocator.h"

#ifndef MAP_INIT_SIZE
#define MAP_INIT_SIZE 10
#endif

#ifndef MAP_COLLISION_FACTOR
#define MAP_COLLISION_FACTOR 0.5
#endif

typedef struct _map_bucket_t {
	void *key;
	void *data;
	struct _map_bucket_t *next;
} _MapBucket;

typedef usize (*MapHashFn)(const void *key, usize size);

typedef struct map_t {
	_MapBucket **base;
	usize len;
	usize collisions;
	MapHashFn hash_fn;
	Object key_obj;
	Object data_obj;
} Map;

// Allocates the backing data for a Map
Map mapNew(Allocator allocator, MapHashFn hash_fn, Object key_obj,
		   Object data_obj);

// Inserts a new element into the Map, copies the data
void mapInsert(Allocator allocator, Map *map, const void *key,
			   const void *data);

// Fetches an element from the Map, returns NULL if not found
void *mapGet(Map map, const void *key);

// Removes an element from the Map
void mapRemove(Allocator allocator, Map *map, const void *key);

// Frees the entire Map
void mapDelete(Allocator allocator, Map map);

// Generates a hash from the raw bytes of the key
usize mapDefaultHash(const void *key, usize size);
