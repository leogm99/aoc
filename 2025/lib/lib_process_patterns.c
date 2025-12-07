#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <regex.h>
#include "lib_mmap.h"
#include "lib_process_patterns.h"

int process_patterns(mapped_file_t mapped_file, pattern_yield_context_t* yield_ctx) {
	regex_t regex;
	const char *pattern = yield_ctx->regex_pattern;
	int c = 0;
	if ((c = regcomp(&regex, pattern, REG_EXTENDED) != 0)) {
		fprintf(stderr, "Error compiling regex: %d\n", c);
		return -1;
	}
	char* input_ptr = mapped_file.region_start;
	char* input_end = mapped_file.region_end;
	int remaining = mapped_file.size;
	while (remaining > 0) {
		regmatch_t pmatch[10] = {0};
		int x = regnexec(&regex, input_ptr, remaining, 10, pmatch, 0);
		if (x == REG_NOMATCH) {
			break;
		}
		for (int i = 0; i < 10; ++i) {
			regmatch_t match = pmatch[i];
			if (match.rm_so == -1) {
				break;
			}
			yield_ctx->yield(yield_ctx->data, &input_ptr[match.rm_so], match.rm_eo - match.rm_so);
			input_ptr += match.rm_eo;
			remaining = input_end - input_ptr;
		}
	}
	regfree(&regex);
	return 0;	
}

