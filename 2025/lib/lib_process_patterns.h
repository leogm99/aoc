#ifndef __LIB_PROCESS_PATTERNS_H__
#define __LIB_PROCESS_PATTERNS_H__

typedef void (*yield_pattern_cb_t)(void* data, char* match, size_t match_size);

typedef struct {
	void* data;
	char* regex_pattern;
	yield_pattern_cb_t yield;
} pattern_yield_context_t;

int process_patterns(mapped_file_t mapped_file, pattern_yield_context_t* yield_ctx);

#endif // __LIB_PROCESS_PATTERNS_H__

