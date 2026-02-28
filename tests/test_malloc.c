#include <criterion/criterion.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../src/malloc.h"

Test(calloc_tests, calloc_basic)
{
    void *ptr = calloc(10, sizeof(int));
    cr_assert_not_null(ptr, "calloc NULL");
    for (size_t i = 0; i < 10 * sizeof(int); i++)
    {
        cr_assert_eq(((unsigned char *)ptr)[i], 0, "calloc error");
    }
    free(ptr);
}
Test(calloc_tests, calloc_zero_elements)
{
    void *ptr = calloc(0, sizeof(int));
    cr_assert_null(ptr, "calloc NULL");
}

Test(calloc_tests, calloc_zero_size)
{
    void *ptr = calloc(10, 0);
    cr_assert_null(ptr, "calloc NULL");
}
Test(calloc_tests, calloc_large)
{
    void *ptr = calloc(1024, 1024 * sizeof(int));
    cr_assert_not_null(ptr, "calloc error");
    free(ptr);
}

Test(calloc_tests, calloc_max_size)
{
    size_t max_size = (SIZE_MAX / 2) / sizeof(int);
    void *ptr = calloc(max_size, sizeof(int));
    cr_assert_null(ptr, "calloc error");
}

Test(calloc_tests, calloc_alignment)
{
    void *ptr = calloc(10, sizeof(int));
    cr_assert_not_null(ptr, "calloc NULL");
    cr_assert(((uintptr_t)ptr % sizeof(long double)) == 0, "calloc error");
    free(ptr);
}
Test(malloc_tests, malloc_basic)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");
    memset(ptr, 1, 100);
    free(ptr);
}

Test(malloc_tests, malloc_zero)
{
    void *ptr = malloc(0);
    cr_assert_null(ptr, "malloc NULL");
}

Test(malloc_tests, malloc_large)
{
    void *ptr = malloc(1024 * 1024);
    cr_assert_not_null(ptr, "malloc error");
    free(ptr);
}

Test(malloc_tests, malloc_alignment)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");
    cr_assert(((uintptr_t)ptr % sizeof(long double)) == 0, "malloc error ");
    free(ptr);
}
Test(realloc_tests, realloc_exp)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");
    memset(ptr, 1, 100);

    void *new_ptr = realloc(ptr, 200);
    cr_assert_not_null(new_ptr, "realloc returned NULL");
    char *data = (char *)new_ptr;
    for (size_t i = 0; i < 100; i++)
    {
        cr_assert_eq(data[i], 1, "realloc error");
    }
    free(new_ptr);
}

Test(realloc_tests, realloc_shr)
{
    void *ptr = malloc(200);
    cr_assert_not_null(ptr, "malloc NULL");
    memset(ptr, 1, 200);

    void *new_ptr = realloc(ptr, 100);
    cr_assert_not_null(new_ptr, "realloc NULL");
    char *data = (char *)new_ptr;
    for (size_t i = 0; i < 100; i++)
    {
        cr_assert_eq(data[i], 1, "realloc error");
    }
    free(new_ptr);
}

Test(realloc_tests, realloc_null)
{
    void *ptr = realloc(NULL, 100);
    cr_assert_not_null(ptr, "realloc NULL");
    free(ptr);
}

Test(realloc_tests, realloc_zero)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");

    void *new_ptr = realloc(ptr, 0);
    cr_assert_null(new_ptr, "realloc NULL");
}
Test(realloc_tests, realloc_very_large)
{
    void *ptr = malloc(1024 * 1024 * 10);
    cr_assert_not_null(ptr, "malloc NULL");

    void *new_ptr = realloc(ptr, 1024 * 1024 * 100);
    cr_assert_not_null(new_ptr, "realloc error");
    free(new_ptr);
}
Test(realloc_tests, realloc_smaller_than_alignment)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");

    void *new_ptr = realloc(ptr, 8);
    cr_assert_not_null(new_ptr, "realloc NULL");
    cr_assert(((uintptr_t)new_ptr % sizeof(long double)) == 0,
              "realloc  error");
    free(new_ptr);
}

Test(realloc_tests, realloc_contiguous_merge)
{
    void *ptr1 = malloc(100);
    cr_assert_not_null(ptr1, "malloc NULL");

    void *ptr2 = malloc(200);
    cr_assert_not_null(ptr2, "malloc NULL");

    free(ptr2);

    void *new_ptr = realloc(ptr1, 300);
    cr_assert_not_null(new_ptr, "realloc NULL");
    free(new_ptr);
}
Test(realloc_tests, realloc_same_size)
{
    void *ptr = malloc(100);
    cr_assert_not_null(ptr, "malloc NULL");

    void *new_ptr = realloc(ptr, 100);
    cr_assert_eq(ptr, new_ptr, "realloc erorr");
    free(new_ptr);
}
