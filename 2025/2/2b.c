#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lib_mmap.h"
#include "lib_process_patterns.h"

typedef struct {
	long long wrong_sum;	
} process_pattern_data_t;

// keeping it easy
int num_digits(long num) {
	return floor(log10(labs(num))) + 1;
}

void process_pattern(void* data, char* match, size_t match_size) {
	process_pattern_data_t* ctx = data;
	char temp[100] = {0};
	memcpy(temp, match, match_size);
	char* dash_ptr = strstr(temp, "-");
	long first_number = strtol(temp, NULL, 10);
	long second_number = strtol(dash_ptr + 1, NULL, 10);

	while (first_number <= second_number)  {
		int digits_first = num_digits(first_number);
		for (int i = 1; i <= digits_first / 2; ++i) {
			int all_equal = 1;
			long temp_num = first_number;
			long rem = temp_num % (long) pow(10.0, i);
			do {
				temp_num = temp_num / (long) pow(10.0, i);
			} while (rem == temp_num % (long) pow(10.0, i) );
			long repeating_pattern = first_number % (long) pow(10., i);
			if (num_digits(repeating_pattern) < i) { 
				continue;
			}
			if (temp_num == 0) {
				ctx->wrong_sum += first_number;	
				break;
			}
		}
		++first_number;
	}
}

int main() {
	mapped_file_t mapped_file = {0};
	int mmap_ret = mmap_file_init("input.txt", &mapped_file);
	if (mmap_ret == -1) {
		fprintf(stderr, "Failed mapping file to memory region\n");
		return -1;
	}
	process_pattern_data_t data = {.wrong_sum = 0};
	pattern_yield_context_t ctx = { .data = &data, .yield = process_pattern, .regex_pattern = "[0-9]+-[0-9]+" };
	if (process_patterns(mapped_file, &ctx) < 0) {
		fprintf(stderr, "Failed processing\n");
		mmap_file_destroy(&mapped_file);
		return -1;
	}
	printf("wrong sum: %lld\n", data.wrong_sum);

	mmap_file_destroy(&mapped_file);
}
