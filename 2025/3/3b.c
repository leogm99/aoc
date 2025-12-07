#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "lib_mmap.h"
#include "lib_process_patterns.h"

typedef struct {
	long total_joltage;	
} process_pattern_data_t;

// find a pointer to the largest number in the range start-end
char* argmax(char* start, char* end) {
	int max = INT_MIN;
	char* max_ptr = NULL;
	char* l_start = start;
	do { 
		char num = *l_start;
		if (num - '0' > max) {
			max = num - '0'	;
			max_ptr = l_start;
		}
	}
	while(++l_start <= end);
	return max_ptr;
}

void process_pattern(void* data, char* bank, size_t bank_size) {
	process_pattern_data_t* ctx = data;
	char temp[101] = {0};
	memcpy(temp, bank, bank_size);
	size_t bank_length = strnlen(temp, 100);
	
	char max_slice[13] = {0};
	// copy left to right starting from the bottom
	memcpy(max_slice, &temp[bank_length - 12], 12);
	char* start = temp;	
	char* end = &temp[bank_length];
	for (int i = 0; i < 12; ++i) {
		char* max = argmax(start, end - 12 + i);
		max_slice[i] = *max;
		start = ++max;
	}	
	long max_joltage = strtol(max_slice, NULL, 10);
	ctx->total_joltage += max_joltage;
}

int main() {
	mapped_file_t mapped_file = {0};
	int mmap_ret = mmap_file_init("input.txt", &mapped_file);
	if (mmap_ret == -1) {
		fprintf(stderr, "Failed mapping file to memory region\n");
		return -1;
	}
	process_pattern_data_t data = { .total_joltage = 0 };
	pattern_yield_context_t ctx = { .data = &data, .yield = process_pattern, .regex_pattern = "[0-9]+" };
	if (process_patterns(mapped_file, &ctx) < 0) {
		fprintf(stderr, "Failed processing\n");
		mmap_file_destroy(&mapped_file);
		return -1;
	}
	printf("total joltage: %ld\n", data.total_joltage);
	mmap_file_destroy(&mapped_file);
}
