#include "lib_arena.h"
#include <stdlib.h>
#include <assert.h>

arena_t* arena_init(size_t capacity) {
	assert((capacity > ARENA_SIZE) && "Arena capacity is too small");
	arena_t* arena = malloc(capacity);
	arena->capacity = capacity - ARENA_SIZE;
	arena->offset = ARENA_SIZE;
	return arena;
}

void* arena_alloc(arena_t* arena, size_t size) {
	if (arena->offset + size > arena->capacity) {
		return NULL;
	}
	size_t offset = arena->offset;
	arena->offset += size;
	return (void*) arena + offset;
}

void arena_destroy(arena_t *arena) {
	if (arena == NULL) {
		return;
	}
	free(arena);
	arena = NULL;
}
