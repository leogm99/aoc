CC = gcc
CFLAGS = -O3 -I./lib
LDFLAGS = -L./lib
LIBS = -laoc_lib

LIB_DIR = ./lib

LIB_SRCS = $(LIB_DIR)/lib_mmap.c $(LIB_DIR)/lib_process_patterns.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_TARGET = $(LIB_DIR)/libaoc_lib.a

PROBLEM_DIRS = $(wildcard [0-9]*)
EXECUTABLES = $(PROBLEM_DIRS)

all: $(LIB_TARGET) $(EXECUTABLES)

$(LIB_TARGET): $(LIB_OBJS)
	ar rcs $@ $^

$(EXECUTABLES): %: $(LIB_TARGET)
	$(CC) $(CFLAGS) -o $@/$@ $@/*.c $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

PROBLEM ?= 1
run: $(PROBLEM)
	./$(PROBLEM)/$(PROBLEM) $(PROBLEM)/input.txt

clean:
	rm -f $(LIB_OBJS) $(LIB_TARGET)
	for dir in $(PROBLEM_DIRS); do rm -f $dir/$dir; done

.PHONY: all run clean

