#include "malloc.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE 4096

static size_t ALIGNMENT = sizeof(long double);
static struct Block *head = NULL;

static struct Block *get_block_ptr(void *ptr)
{
    return (struct Block *)((char *)ptr - sizeof(struct Block));
}

static void initialize(void)
{
    void *ptr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        return;
    }
    head = ptr;
    head->size = PAGE_SIZE - sizeof(struct Block);
    head->free = 1;
    head->next = NULL;
}

static size_t align_size(size_t size)
{
    size_t var = ALIGNMENT - 1;
    size_t aligned_size;

    if (__builtin_add_overflow(size, var, &aligned_size))
    {
        return 0;
    }
    aligned_size = aligned_size & ~(ALIGNMENT - 1);

    return aligned_size;
}

static void split(struct Block *current, size_t aligned_size)
{
    if (current->size >= aligned_size + sizeof(struct Block) + 1)
    {
        void *ptr = (char *)current + sizeof(struct Block) + aligned_size;
        struct Block *new_block = ptr;

        new_block->size = current->size - aligned_size - sizeof(struct Block);
        new_block->free = 1;
        new_block->next = current->next;

        current->size = aligned_size;
        current->free = 0;
        current->next = new_block;
    }
    else
    {
        current->free = 0;
    }
}

static struct Block *extend_heap(size_t aligned_size)
{
    size_t total_size = aligned_size > PAGE_SIZE - sizeof(struct Block)
        ? ((aligned_size / PAGE_SIZE) + 1) * PAGE_SIZE
        : PAGE_SIZE;
    void *ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
    {
        return NULL;
    }
    struct Block *new_block = ptr;
    new_block->size = total_size - sizeof(struct Block);
    new_block->free = 1;
    new_block->next = NULL;

    struct Block *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_block;

    if (new_block->size >= aligned_size)
    {
        split(new_block, aligned_size);
        return new_block;
    }
    return NULL;
}

__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (size == 0)
    {
        return NULL;
    }
    size_t aligned_size = align_size(size);
    if (aligned_size == 0)
    {
        return NULL;
    }
    if (head == NULL)
    {
        initialize();
        if (head == NULL)
        {
            return NULL;
        }
    }
    struct Block *current = head;
    while (current != NULL)
    {
        if (current->free == 1 && current->size >= aligned_size)
        {
            split(current, aligned_size);
            return (char *)current + sizeof(struct Block);
        }
        current = current->next;
    }
    current = extend_heap(aligned_size);
    if (current == NULL)
    {
        return NULL;
    }
    split(current, aligned_size);
    return (char *)current + sizeof(struct Block);
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    struct Block *block = get_block_ptr(ptr);
    block->free = 1;
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);
    if (size > SIZE_MAX - sizeof(struct Block) || size == 0)
    {
        free(ptr);
        return NULL;
    }
    struct Block *block = get_block_ptr(ptr);
    if (block == NULL)
        return NULL;
    size_t aligned_size = align_size(size);
    if (aligned_size == 0)
        return NULL;
    if (block->size >= aligned_size)
    {
        split(block, aligned_size);
        return ptr;
    }
    else
    {
        if (block->next != NULL && block->next->free
            && (block->size + sizeof(struct Block) + block->next->size)
                >= aligned_size)
        {
            if (block->size + block->next->size
                > SIZE_MAX - sizeof(struct Block))
            {
                return NULL;
            }
            block->size += sizeof(struct Block) + block->next->size;
            block->next = block->next->next;
            split(block, aligned_size);
            return ptr;
        }
        else
        {
            void *new_ptr = malloc(size);
            if (new_ptr == NULL)
            {
                return NULL;
            }
            memcpy(new_ptr, ptr, block->size);
            free(ptr);
            return new_ptr;
        }
    }
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    if (nmemb == 0 || size == 0)
    {
        return NULL;
    }
    if (nmemb > SIZE_MAX / size)
    {
        return NULL;
    }

    size_t total_size = nmemb * size;

    void *ptr = malloc(total_size);
    if (ptr == NULL)
    {
        return NULL;
    }

    memset(ptr, 0, total_size);

    return ptr;
}
