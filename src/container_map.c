#include <string.h>

#include "container/map.h"
#include "container/string.h"

Map mapNew(Allocator allocator, MapHashFn hash_fn, MapKeyCmp key_eq) {
	_MapBucket **base = new(allocator, _MapBucket *, MAP_INIT_SIZE);
	memset(base, 0, sizeof(_MapBucket *));
	return (Map){
		.base = base,
		.len = MAP_INIT_SIZE,
		.collisions = 0,
		.hash_fn = hash_fn,
		.key_eq = key_eq,
	};
}

void mapInsert(Allocator allocator, Map *map, void *key, usize key_size,
			   void *data) {
	usize hash = map->hash_fn(key, key_size);
	usize idx = hash % map->len;
	_MapBucket *bucket = new(allocator, _MapBucket, 1);
	bucket->key = key;
	bucket->data = data;
	bucket->next = NULL;
	if ((f32)map->collisions > (f32)map->len * MAP_COLLISION_FACTOR) {
		map->len *= 2;
		map->base =
			resize(allocator, map->len * sizeof(_MapBucket *), map->base);
		idx = hash % map->len;
	}
	_MapBucket *curr = map->base[idx];
	if (!curr) {
		map->base[idx] = bucket;
		return;
	}
	map->collisions++;
	while (curr->next) {
		curr = curr->next;
	}
	curr->next = bucket;
}

void *mapGet(Map map, void *key, usize size) {
	usize idx = map.hash_fn(key, size) % map.len;
	_MapBucket *curr = map.base[idx];
	while (curr) {
		if (map.key_eq(key, size, curr->key, curr->key_size))
			return curr->data;
		curr = curr->next;
	}
	return NULL;
}

void mapRemove(Allocator allocator, Map *map, void *key, usize size) {
	map->collisions--;
	usize idx = map->hash_fn(key, size);
	_MapBucket *curr = map->base[idx];
	_MapBucket *prev = curr;
	while (curr) {
		if (map->key_eq(key, size, curr->key, curr->key_size)) {
			if (curr == map->base[idx])
				map->base[idx] = curr->next;
			else
				prev->next = curr->next;
			delete(allocator, curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

void mapDelete(Allocator allocator, Map map) {
	for (usize i = 0; i < map.len; i++) {
		_MapBucket *curr = map.base[i];
		if (curr) {
			_MapBucket *prev = curr;
			curr = curr->next;
			while (curr) {
				delete(allocator, prev);
				prev = curr;
				curr = curr->next;
			}
			delete(allocator, prev);
		}
	}
	delete(allocator, map.base);
}

#define MAGIC_PRIME	 5381
#define MAGIC_NUMBER 5

usize mapDefaultHash(void *key, usize size) {
	byte *p = key;
	usize hash = MAGIC_PRIME;
	for (usize i = 0; i < size; i++)
		hash = ((hash << MAGIC_NUMBER) + hash) + p[i];
	return hash;
}

bool mapKeyCmpString(void *key1, usize, void *key2, usize) {
	String str1 = *(String *)key1;
	String str2 = *(String *)key2;
	return stringCmp(str1, str2) == 0;
}

bool mapKeyEqInt64(void *key1, usize, void *key2, usize) {
	i64 k1 = *(i64 *)key1;
	i64 k2 = *(i64 *)key2;
	return k1 == k2;
}
