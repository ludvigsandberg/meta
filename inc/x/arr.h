#ifndef XARR_H
#define XARR_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <x/meta.h>

#define xarr(T) T *

// array length
#define xalen(A) ((size_t *)(A))[-2]

// array capacity
#define xacap(A) ((size_t *)(A))[-1]

// create empty array
#define xarr_new(A)                                                           \
    xgeneric_assign((A), (size_t *)(calloc(2, sizeof(size_t))) + 2);

// create array with n elements
#define xarr_new_n(A, N)                                                      \
    do {                                                                      \
        xgeneric_assign(                                                      \
            (A),                                                              \
            (size_t *)malloc(2 * sizeof(size_t) + (N) * sizeof((A)[0])) + 2); \
        xalen(A) = xacap(A) = (N);                                            \
    } while (0);

// create array with n elements set to zero
#define xarr_new_n_zero(A, N)                                                 \
    do {                                                                      \
        xgeneric_assign((A), (size_t *)calloc(1, 2 * sizeof(size_t) +         \
                                                     (N) * sizeof((A)[0])) +  \
                                 2);                                          \
        xalen(A) = xacap(A) = (N);                                            \
    } while (0);

// create array and reserve n elements
#define xarr_new_reserve(A, N)                                                \
    do {                                                                      \
        xgeneric_assign(                                                      \
            (A),                                                              \
            (size_t *)malloc(2 * sizeof(size_t) + (N) * sizeof((A)[0])) + 2); \
        xalen(A) = 0;                                                         \
        xacap(A) = (N);                                                       \
    } while (0);

// create array and reserve n elements set to zero
#define xarr_new_reserve_zero(A, N)                                           \
    do {                                                                      \
        xgeneric_assign((A), (size_t *)calloc(1, 2 * sizeof(size_t) +         \
                                                     (N) * sizeof((A)[0])) +  \
                                 2);                                          \
        xalen(A) = 0;                                                         \
        xacap(A) = (N);                                                       \
    } while (0);

// free array
#define xarr_free(A) free(((size_t *)A) - 2);

// resize array
#define xarr_resize(A, N)                                                     \
    do {                                                                      \
        xgeneric_assign((A), (size_t *)(realloc((size_t *)(A) - 2,            \
                                                2 * sizeof(size_t) +          \
                                                    (N) * sizeof((A)[0]))) +  \
                                 2);                                          \
        xalen(A) = xacap(A) = (N);                                            \
    } while (0)

// insert n uninitialized elements into array at index
#define xarr_insert_raw_n(A, I, N)                                            \
    do {                                                                      \
        assert((I) <= xalen(A));                                              \
                                                                              \
        if (xalen(A) + (N) > xacap(A)) {                                      \
            xacap(A) = (xalen(A) + (N)) * 2;                                  \
                                                                              \
            xgeneric_assign(                                                  \
                (A), (size_t *)(realloc((size_t *)(A) - 2,                    \
                                        2 * sizeof(size_t) +                  \
                                            xacap(A) * sizeof((A)[0]))) +     \
                         2);                                                  \
        }                                                                     \
                                                                              \
        if ((I) != xalen(A)) {                                                \
            memmove((A) + (I) + (N), (A) + (I),                               \
                    (xalen(A) - (I)) * sizeof((A)[0]));                       \
        }                                                                     \
                                                                              \
        xalen(A) += (N);                                                      \
    } while (0)

// insert uninitialized element into array at index
#define xarr_insert_raw(A, I) xarr_insert_raw_n(A, I, 1)

// append n uninitialized elements to array
#define xarr_append_n_raw(A, N)                                               \
    do {                                                                      \
        /* capture I before xalen(A) mutates */                               \
        size_t xuniq(i) = xalen(A);                                           \
        xarr_insert_raw_n(A, xuniq(i), N);                                    \
    } while (0)

// append uninitialized element to array
#define xarr_append_raw(A) xarr_append_n_raw(A, 1)

// insert n elements into array at index
#define xarr_insert_n(A, I, N, B)                                             \
    do {                                                                      \
        xarr_insert_raw_n(A, I, N);                                           \
                                                                              \
        for (size_t xuniq(j) = 0; xuniq(j) < (N); ++xuniq(j)) {               \
            (A)[(I) + xuniq(j)] = (B)[xuniq(j)];                              \
        }                                                                     \
    } while (0)

// insert element into array at index
#define xarr_insert(A, I, E) xarr_insert_n(A, I, 1, &(E))

// append n elements to array
#define xarr_append_n(A, N, B)                                                \
    do {                                                                      \
        /* capture len before it mutates */                                   \
        size_t xuniq(i) = xalen(A);                                           \
        xarr_insert_n(A, xuniq(i), N, B);                                     \
    } while (0)

// append element to array
#define xarr_append(A, E) xarr_append_n(A, 1, &(E))

// remove n elements from array at index
#define xarr_remove_n(A, I, N)                                                \
    do {                                                                      \
        assert((I) < xalen(A));                                               \
        assert((I) + (N) <= xalen(A));                                        \
                                                                              \
        if ((I) + (N) < xalen(A)) {                                           \
            memmove((A) + (I), (A) + (I) + (N),                               \
                    (xalen(A) - (I) - (N)) * sizeof((A)[0]));                 \
        }                                                                     \
                                                                              \
        xalen(A) -= (N);                                                      \
    } while (0)

// remove element from array at index
#define xarr_remove(A, I) xarr_remove_n(A, I, 1)

// remove element from array at pointer
#define xarr_remove_ptr(A, P) xarr_remove(A, (size_t)((P) - (A)))

#endif
