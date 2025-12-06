#ifndef __LIB_MMAP__
#define  __LIB_MMAP__

typedef struct {
	char* region_start;
	char* region_end;
	size_t size;
} mapped_file_t;

int mmap_file_init(const char* path, mapped_file_t* mapped_file);
void mmap_file_destroy(mapped_file_t* mapped_file);

#endif // __LIB_MMAP__

