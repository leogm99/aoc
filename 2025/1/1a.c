#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lib_mmap.h"
#include "lib_process_patterns.h"

typedef struct {
	int dial;	
	int	zeros; 
} process_pattern_data_t;

void process_pattern(void* ctx_data, char* match, size_t match_size) {
	process_pattern_data_t* data = ctx_data;

	// temp buffer just because no strntol exists in stdlib and im not willing to write it myself :p
	char temp[100] = {0};
	memcpy(temp, &match[1], match_size - 1);
	long num = strtol(temp, NULL, 10);
	char direction = match[0];
	if (direction == 'L') {
		data->dial = (data->dial - num) % 100;
	}
	else if (direction == 'R') {
		data->dial = (data->dial + num) % 100;
	}
	if (data->dial == 0) {
		data->zeros += 1;
	}
}

int main(void) {
	mapped_file_t mapped_file = {0};
	int mmap_ret = mmap_file_init("input.txt", &mapped_file);
	if (mmap_ret == -1) {
		fprintf(stderr, "Failed mapping file to memory region\n");
		return -1;
	}
	process_pattern_data_t data = {.dial = 50, .zeros = 0};
	pattern_yield_context_t ctx = { .data = &data, .yield = process_pattern, .regex_pattern = "[LR]\\d*\\n" };
	if (process_patterns(mapped_file, &ctx) < 0) {
		fprintf(stderr, "Failed processing\n");
		mmap_file_destroy(&mapped_file);
		return -1;
	}
	printf("zeros: %d\n", data.zeros);

	mmap_file_destroy(&mapped_file);
	return 0;
}

