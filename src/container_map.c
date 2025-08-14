#include <string.h>

#include "container/map.h"

Map mapNew(Allocator allocator, MapHashFn hash_fn, Object key_obj,
		   Object data_obj) {
	_MapBucket **base = new(allocator, _MapBucket *, MAP_INIT_SIZE);
	memset(base, 0, sizeof(_MapBucket *));
	return (Map){
		.base = base,
		.len = MAP_INIT_SIZE,
		.collisions = 0,
		.hash_fn = hash_fn,
		.key_obj = key_obj,
		.data_obj = data_obj,
	};
}

void mapInsert(Allocator allocator, Map *map, const void *key,
			   const void *data) {
	usize hash = map->hash_fn(key, map->key_obj.size());
	usize idx = hash % map->len;
	_MapBucket *bucket = new(allocator, _MapBucket, 1);
	bucket->key = map->key_obj.clone(allocator, key);
	bucket->data = map->data_obj.clone(allocator, data);
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

void *mapGet(Map map, const void *key) {
	usize idx = map.hash_fn(key, map.key_obj.size()) % map.len;
	_MapBucket *curr = map.base[idx];
	while (curr) {
		if (!map.key_obj.compare(key, curr->key)) {
			return curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}

void mapRemove(Allocator allocator, Map *map, const void *key) {
	map->collisions--;
	usize idx = map->hash_fn(key, map->key_obj.size());
	_MapBucket *curr = map->base[idx];
	_MapBucket *prev = curr;
	while (curr) {
		if (!map->key_obj.compare(key, curr->key)) {
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
				map.key_obj.free(allocator, prev->key);
				map.data_obj.free(allocator, prev->data);
				delete(allocator, prev);
				prev = curr;
				curr = curr->next;
			}
			map.key_obj.free(allocator, prev->key);
			map.data_obj.free(allocator, prev->data);
			delete(allocator, prev);
		}
	}
	delete(allocator, map.base);
}

#define MAGIC_PRIME	 5381
#define MAGIC_NUMBER 5

usize mapDefaultHash(const void *key, usize size) {
	const byte *p = key;
	usize hash = MAGIC_PRIME;
	for (usize i = 0; i < size; i++)
		hash = ((hash << MAGIC_NUMBER) + hash) + p[i];
	return hash;
}
