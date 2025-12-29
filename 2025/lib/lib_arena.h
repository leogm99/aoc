#ifndef __LIB_ARENA__
#define __LIB_ARENA__

#include <stddef.h>

typedef struct {
	size_t offset;
	size_t capacity;
} arena_t;

#define ARENA_SIZE sizeof(arena_t)

arena_t* arena_init(size_t capacity);
void* arena_alloc(arena_t* arena, size_t size);
void arena_destroy(arena_t* arena);

#endif
