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

void process_pattern(void* data, char* bank, size_t bank_size) {
	process_pattern_data_t* ctx = data;
	char temp[101] = {0};
	memcpy(temp, bank, bank_size);
	int max_joltage = INT_MIN;
	for (int i = 0; i < bank_size; ++i) {
		// hackish, but works
		int first_battery = bank[i] - '0';
		for (int j = i + 1; j < bank_size; ++j) { 
			int second_battery = bank[j] - '0';
			int joltage = 10 * first_battery + second_battery;
			max_joltage = max_joltage < joltage ? joltage : max_joltage;
		}
	}
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
