#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/malloc.h"

int main()
{
    size_t alignment = sizeof(long double);
    void *ptr1 = malloc(100);
    if (ptr1 != NULL && ((uintptr_t)ptr1 % alignment) == 0)
    {
        memset(ptr1, 1, 100);
        printf("ok: malloc\n");
    }
    else
    {
        printf("ko: malloc\n");
    }
    free(ptr1);

    void *ptr2 = malloc(0);
    if (ptr2 == NULL)
    {
        printf("ok: malloc\n");
    }
    else
    {
        printf("ko: malloc\n");
        free(ptr2);
    }
    void *ptr3 = malloc(1024 * 1024 * 10);
    if (ptr3 != NULL && ((uintptr_t)ptr3 % alignment) == 0)
    {
        memset(ptr3, 1, 1024 * 1024 * 10);
        printf("ok: malloc\n");
    }
    else
    {
        printf("ko: malloc\n");
    }
    free(ptr3);
    void *ptr4 = calloc(50, 2);
    if (ptr4 != NULL && ((uintptr_t)ptr4 % alignment) == 0)
    {
        int all_zero = 1;
        char *cptr = ptr4;
        for (size_t i = 0; i < 100; i++)
        {
            if (cptr[i] != 0)
            {
                all_zero = 0;
                break;
            }
        }
        if (all_zero)
        {
            printf("ok: calloc\n");
        }
        else
        {
            printf("ko: calloc\n");
        }
        free(ptr4);
    }
    else
    {
        printf("ko: calloc\n");
    }
    void *ptr5 = malloc(100);
    if (ptr5 != NULL && ((uintptr_t)ptr5 % alignment) == 0)
    {
        memset(ptr5, 1, 100);
        void *new_ptr5 = realloc(ptr5, 200);
        if (new_ptr5 != NULL && ((uintptr_t)new_ptr5 % alignment) == 0)
        {
            int preserved = 1;
            char *cptr5 = new_ptr5;
            for (size_t i = 0; i < 100; i++)
            {
                if (cptr5[i] != 1)
                {
                    preserved = 0;
                    break;
                }
            }
            if (preserved)
            {
                printf("ok: realloc\n");
            }
            else
            {
                printf("ko: realloc\n");
            }
            free(new_ptr5);
        }
        else
        {
            printf("ko: realloc\n");
            free(ptr5);
        }
    }
    else
    {
        printf("ko: realloc\n");
    }

    void *ptr6 = malloc(200);
    if (ptr6 != NULL && ((uintptr_t)ptr6 % alignment) == 0)
    {
        memset(ptr6, 1, 200);
        void *new_ptr6 = realloc(ptr6, 100);
        if (new_ptr6 != NULL && ((uintptr_t)new_ptr6 % alignment) == 0)
        {
            int preserved = 1;
            char *cptr6 = new_ptr6;
            for (size_t i = 0; i < 100; i++)
            {
                if (cptr6[i] != 1)
                {
                    preserved = 0;
                    break;
                }
            }
            if (preserved)
            {
                printf("ok: realloc\n");
            }
            else
            {
                printf("ko: realloc\n");
            }
            free(new_ptr6);
        }
        else
        {
            printf("ko: realloc\n");
            free(ptr6);
        }
    }
    else
    {
        printf("ko: realloc\n");
    }

    free(NULL);
    printf("ok: free\n");
    void *ptr9 = realloc(NULL, 150);
    if (ptr9 != NULL && ((uintptr_t)ptr9 % alignment) == 0)
    {
        printf("ok: realloc\n");
        free(ptr9);
    }
    else
    {
        printf("ko: realloc\n");
    }
    void *ptr10 = malloc(100);
    if (ptr10 != NULL && ((uintptr_t)ptr10 % alignment) == 0)
    {
        void *new_ptr10 = realloc(ptr10, 0);
        if (new_ptr10 == NULL)
        {
            printf("ok: realloc\n");
        }
        else
        {
            printf("ko: realloc\n");
            free(new_ptr10);
        }
    }
    else
    {
        printf("ko: realloc\n");
    }

    return 0;
}
