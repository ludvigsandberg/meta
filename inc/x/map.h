#ifndef XMAP_H
#define XMAP_H

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <x/arr.h>

#define xmap(K, V)                                                            \
    struct {                                                                  \
        xarr(xarr(struct {                                                    \
            K key;                                                            \
            V val;                                                            \
        })) bkts;                                                             \
                                                                              \
        size_t len; /* num entries */                                         \
        size_t pop; /* num populated buckets */                               \
    }

// map length
#define xmap_len(M) (M).len

// iterate over key value pairs in map
#define xmap_foreach(M, K, V)                                                 \
    for (size_t xuniq(i) = 0; xuniq(i) < xalen((M).bkts); ++xuniq(i))         \
        for (size_t xuniq(j) = 0; xuniq(j) < xalen((M).bkts[xuniq(i)]);       \
             ++xuniq(j))                                                      \
            for (bool xuniq(once) = true; xuniq(once);)                       \
                for ((K) = &((M).bkts[xuniq(i)][xuniq(j)].key),               \
                    (V)  = &((M).bkts[xuniq(i)][xuniq(j)].val);               \
                     xuniq(once); xuniq(once) = false)

// create map with initial capacity
#define xmap_new_reserve(M, N)                                                \
    do {                                                                      \
        xarr_new_n((M).bkts, N);                                              \
        (M).len = (M).pop = 0;                                                \
                                                                              \
        for (size_t xuniq2(i, xmap_new_reserve) = 0;                          \
             xuniq2(i, xmap_new_reserve) < xalen((M).bkts);                   \
             ++xuniq2(i, xmap_new_reserve)) {                                 \
            xarr_new_reserve((M).bkts[xuniq2(i, xmap_new_reserve)], 2);       \
        }                                                                     \
    } while (0)

// create map
#define xmap_new(M) xmap_new_reserve(M, 16)

// free map
#define xmap_free(M)                                                          \
    do {                                                                      \
        for (size_t xuniq(i) = 0; xuniq(i) < xalen((M).bkts); ++xuniq(i)) {   \
            xarr_free((M).bkts[xuniq(i)]);                                    \
        }                                                                     \
        xarr_free((M).bkts);                                                  \
    } while (0)

// get map value, null if not found
#define xmap_get_maybe_null(M, HASH, EQ, K, V)                                \
    do {                                                                      \
        (V)                = NULL;                                            \
        size_t xuniq(hash) = HASH(&(K)) % xacap((M).bkts);                    \
                                                                              \
        for (size_t xuniq(i) = 0; xuniq(i) < xalen((M).bkts[xuniq(hash)]);    \
             ++xuniq(i)) {                                                    \
            if (EQ(&(M).bkts[xuniq(hash)][xuniq(i)].key, &(K))) {             \
                (V) = &(M).bkts[xuniq(hash)][xuniq(i)].val;                   \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

// get map value
#define xmap_get(M, HASH, EQ, K, V)                                           \
    do {                                                                      \
        xmap_get_maybe_null(M, HASH, EQ, K, V);                               \
        assert(V);                                                            \
    } while (0)

// check if key exists in map
#define xmap_contains(M, HASH, EQ, K, BOOL)                                   \
    do {                                                                      \
        void *xuniq2(v, xmap_contains);                                       \
        xmap_get_maybe_null(M, HASH, EQ, K, xuniq2(v, xmap_contains));        \
        (BOOL) = xuniq2(v, xmap_contains) != NULL;                            \
    } while (0)

// set map key without rehash
#define _xmap_set_no_rehash(M, CAP, HASH, EQ, K, V)                           \
    do {                                                                      \
        size_t xuniq2(hash, xmap_set_no_rehash)   = HASH(&(K)) % (CAP);       \
        bool xuniq2(replaced, xmap_set_no_rehash) = false;                    \
                                                                              \
        for (size_t xuniq2(i, xmap_set_no_rehash) = 0;                        \
             xuniq2(i, xmap_set_no_rehash) <                                  \
             xalen((M).bkts[xuniq2(hash, xmap_set_no_rehash)]);               \
             ++xuniq2(i, xmap_set_no_rehash)) {                               \
            if (EQ(&(M).bkts[xuniq2(hash, xmap_set_no_rehash)]                \
                            [xuniq2(i, xmap_set_no_rehash)]                   \
                                .key,                                         \
                   &(K))) {                                                   \
                (M).bkts[xuniq2(hash, xmap_set_no_rehash)]                    \
                        [xuniq2(i, xmap_set_no_rehash)]                       \
                            .val                     = V;                     \
                xuniq2(replaced, xmap_set_no_rehash) = true;                  \
                break;                                                        \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (!xuniq2(replaced, xmap_set_no_rehash)) {                          \
            (M).len++;                                                        \
                                                                              \
            if (xalen((M).bkts[xuniq2(hash, xmap_set_no_rehash)]) == 0) {     \
                (M).pop++;                                                    \
            }                                                                 \
                                                                              \
            xarr_append_raw((M).bkts[xuniq2(hash, xmap_set_no_rehash)]);      \
            ((M).bkts[xuniq2(hash, xmap_set_no_rehash)])                      \
                [xalen((M).bkts[xuniq2(hash, xmap_set_no_rehash)]) - 1]       \
                    .key = K;                                                 \
            ((M).bkts[xuniq2(hash, xmap_set_no_rehash)])                      \
                [xalen((M).bkts[xuniq2(hash, xmap_set_no_rehash)]) - 1]       \
                    .val = V;                                                 \
        }                                                                     \
    } while (0)

// set key and value in map
#define xmap_set(M, HASH, EQ, K, V)                                           \
    do {                                                                      \
        /* grow array if needed */                                            \
        if ((M).pop == xacap((M).bkts) / 2) {                                 \
            (M).len = (M).pop                = 0;                             \
            size_t xuniq2(old_cap, xmap_set) = xacap((M).bkts);               \
            size_t xuniq2(new_cap, xmap_set) = xacap((M).bkts) * 2;           \
                                                                              \
            /* double number of buckets and allocate 3rd region to            \
               temporarily store old data while rehashing */                  \
            xarr_resize((M).bkts, xuniq2(new_cap, xmap_set) +                 \
                                      xuniq2(old_cap, xmap_set));             \
                                                                              \
            /* move old data to 3rd region */                                 \
            memcpy((M).bkts + xuniq2(new_cap, xmap_set), (M).bkts,            \
                   (xuniq2(old_cap, xmap_set)) * sizeof((M).bkts[0]));        \
            memset((M).bkts, 0x0,                                             \
                   xuniq2(old_cap, xmap_set) * sizeof((M).bkts[0]));          \
                                                                              \
            /* init new buckets */                                            \
            for (size_t xuniq2(bkt, xmap_set) = 0;                            \
                 xuniq2(bkt, xmap_set) < xuniq2(new_cap, xmap_set);           \
                 ++xuniq2(bkt, xmap_set)) {                                   \
                xarr_new_reserve((M).bkts[xuniq2(bkt, xmap_set)], 2);         \
            }                                                                 \
                                                                              \
            /* rehash */                                                      \
            for (size_t xuniq2(bkt, xmap_set) = 0;                            \
                 xuniq2(bkt, xmap_set) < xuniq2(old_cap, xmap_set);           \
                 ++xuniq2(bkt, xmap_set)) {                                   \
                for (size_t xuniq2(i, xmap_set) = 0;                          \
                     xuniq2(i, xmap_set) <                                    \
                     xalen(                                                   \
                         ((M).bkts +                                          \
                          xuniq2(new_cap, xmap_set))[xuniq2(bkt, xmap_set)]); \
                     ++xuniq2(i, xmap_set)) {                                 \
                    _xmap_set_no_rehash(                                      \
                        M, xuniq2(new_cap, xmap_set), HASH, EQ,               \
                        ((M).bkts + xuniq2(new_cap, xmap_set))[xuniq2(        \
                            bkt, xmap_set)][xuniq2(i, xmap_set)]              \
                            .key,                                             \
                        ((M).bkts + xuniq2(new_cap, xmap_set))[xuniq2(        \
                            bkt, xmap_set)][xuniq2(i, xmap_set)]              \
                            .val);                                            \
                }                                                             \
                                                                              \
                /* free old bucket after rehash  */                           \
                xarr_free(                                                    \
                    ((M).bkts +                                               \
                     xuniq2(new_cap, xmap_set))[xuniq2(bkt, xmap_set)]);      \
            }                                                                 \
                                                                              \
            /* shrink array to new capacity */                                \
            xacap((M).bkts) = xuniq2(new_cap, xmap_set);                      \
        }                                                                     \
                                                                              \
        /* insert new value */                                                \
        _xmap_set_no_rehash(M, xacap((M).bkts), HASH, EQ, K, V);              \
    } while (0)

// remove key value pair from map
#define xmap_remove(M, HASH, EQ, K)                                           \
    do {                                                                      \
        size_t xuniq2(hash, xmap_remove) = HASH(&(K)) % xacap((M).bkts);      \
                                                                              \
        for (size_t xuniq2(i, xmap_remove) = 0;                               \
             xuniq2(i, xmap_remove) <                                         \
             xalen((M).bkts[xuniq2(hash, xmap_remove)]);                      \
             ++xuniq2(i, xmap_remove)) {                                      \
            if (EQ(&(M).bkts[xuniq2(hash, xmap_remove)]                       \
                            [xuniq2(i, xmap_remove)]                          \
                                .key,                                         \
                   &(K))) {                                                   \
                xarr_remove((M).bkts[xuniq2(hash, xmap_remove)],              \
                            xuniq2(i, xmap_remove));                          \
                                                                              \
                (M).len--;                                                    \
                                                                              \
                if (xalen((M).bkts[xuniq2(hash, xmap_remove)]) == 0) {        \
                    (M).pop--;                                                \
                }                                                             \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

#endif
