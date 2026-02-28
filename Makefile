CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -Wvla
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = malloc.o 

all: library tests

library: $(TARGET_LIB)

$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden -fPIC
$(TARGET_LIB): LDFLAGS += -Wl,--no-undefined
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET_LIB) $(OBJS)

tests: tests/test_basic tests/test_malloc

tests/test_basic: tests/test_basic.c $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o tests/test_basic tests/test_basic.c $(OBJS) -L. -lmalloc

tests/test_malloc: tests/test_malloc.c $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o tests/test_malloc tests/test_malloc.c $(OBJS) -L. -lmalloc -lcriterion

debug: CFLAGS += -g
debug: clean library tests

clean:
	$(RM) $(TARGET_LIB) $(OBJS) tests/test_basic tests/test_malloc

.PHONY: all library clean
