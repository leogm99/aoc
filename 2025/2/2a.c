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
	int digits_first = num_digits(first_number);
	int digits_second = num_digits(second_number);
	if (digits_first == digits_second && digits_first % 2 == 1) {
		// odd ranges are ok
		return;
	}
	while (first_number <= second_number)  {
		digits_first = num_digits(first_number);
		if (digits_first % 2 == 1) { 
			long old = first_number;
			first_number = pow((double) 10, digits_first);
			continue;
		}
		long long base_pow = (long long) pow(10., (double) digits_first / 2.);
		long long bottom_half = first_number % base_pow;
		long long upper_half = (long long) ((float) first_number / (float) base_pow);
		if (bottom_half == upper_half) {
			ctx->wrong_sum += first_number;
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
