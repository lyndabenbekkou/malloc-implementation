#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

struct Block
{
    struct Block *next;
    size_t size;
    int free;
    char tab[12];
};

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

#endif /* !MALLOC_H */
