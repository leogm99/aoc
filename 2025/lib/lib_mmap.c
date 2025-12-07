#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

#include "lib_mmap.h"

int mmap_file_init(const char* path, mapped_file_t* mapped_file) {
	int ret = 0;
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "failed to open file\n");
		return -1;
	}
	struct stat buf = {0};
	if (fstat(fd, &buf) < 0) {
		fprintf(stderr, "failed reading file stats\n");
		ret = -1;
		goto free;
	}
	void* region = mmap(0, (int) buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (region == (void*) -1) {
		char* error = strerror(errno);
		fprintf(stderr, "failed mmaping file: %s\n", error);
		ret = -1;
		goto free;
	}
	mapped_file->size = buf.st_size;
	mapped_file->region_start = region;
	mapped_file->region_end = buf.st_size - 1 + region;
free:
	// closing the file is ok, mmap does not care for the file being opened after mmapping (man 2 mmap)
	close(fd);
	return ret;
}

void mmap_file_destroy(mapped_file_t* mapped_file) {
	if (mapped_file->region_start == NULL) {
		return;
	}
	munmap(mapped_file->region_start, mapped_file->size);
	memset(mapped_file, 0, sizeof(*mapped_file));
}

