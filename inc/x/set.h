#ifndef XSET_H
#define XSET_H

#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <x/arr.h>

#define xset(K)                                                               \
    struct {                                                                  \
        xarr(xarr(K)) bkts;                                                   \
                                                                              \
        size_t len; /* num entries */                                         \
        size_t pop; /* num populated buckets */                               \
    }

// set length
#define xset_len(S) (S).len

// create set with initial capacity
#define xset_new_reserve(S, N)                                                \
    do {                                                                      \
        xarr_new_n((S).bkts, N);                                              \
        (S).len = (S).pop = 0;                                                \
                                                                              \
        for (size_t xuniq2(i, xset_new_reserve) = 0;                          \
             xuniq2(i, xset_new_reserve) < xalen((S).bkts);                   \
             ++xuniq2(i, xset_new_reserve)) {                                 \
            xarr_new_reserve((S).bkts[xuniq2(i, xset_new_reserve)], 2);       \
        }                                                                     \
    } while (0)

// create set
#define xset_new(S) xset_new_reserve(S, 16)

// free set
#define xset_free(S)                                                          \
    do {                                                                      \
        for (size_t xuniq(i) = 0; xuniq(i) < xalen((S).bkts); ++xuniq(i)) {   \
            xarr_free((S).bkts[xuniq(i)]);                                    \
        }                                                                     \
        xarr_free((S).bkts);                                                  \
    } while (0)

// check if key exists in set
#define xset_contains(S, HASH, EQ, K, BOOL)                                   \
    do {                                                                      \
        (BOOL) = false;                                                       \
                                                                              \
        size_t xuniq(hash) = HASH(&(K)) % xacap((S).bkts);                    \
                                                                              \
        for (size_t xuniq(i) = 0; xuniq(i) < xalen((S).bkts[xuniq(hash)]);    \
             ++xuniq(i)) {                                                    \
            if (EQ(&(S).bkts[xuniq(hash)][xuniq(i)], &(K))) {                 \
                (BOOL) = true;                                                \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

// set key without rehash
#define _xset_set_no_rehash(S, CAP, HASH, EQ, K)                              \
    do {                                                                      \
        size_t xuniq2(hash, xset_set_no_rehash)   = HASH(&(K)) % (CAP);       \
        bool xuniq2(replaced, xset_set_no_rehash) = false;                    \
                                                                              \
        for (size_t xuniq2(i, xset_set_no_rehash) = 0;                        \
             xuniq2(i, xset_set_no_rehash) <                                  \
             xalen((S).bkts[xuniq2(hash, xset_set_no_rehash)]);               \
             ++xuniq2(i, xset_set_no_rehash)) {                               \
            if (EQ(&(S).bkts[xuniq2(hash, xset_set_no_rehash)]                \
                            [xuniq2(i, xset_set_no_rehash)],                  \
                   &(K))) {                                                   \
                xuniq2(replaced, xset_set_no_rehash) = true;                  \
                break;                                                        \
            }                                                                 \
        }                                                                     \
                                                                              \
        if (!xuniq2(replaced, xset_set_no_rehash)) {                          \
            (S).len++;                                                        \
                                                                              \
            if (xalen((S).bkts[xuniq2(hash, xset_set_no_rehash)]) == 0) {     \
                (S).pop++;                                                    \
            }                                                                 \
                                                                              \
            xarr_append_raw((S).bkts[xuniq2(hash, xset_set_no_rehash)]);      \
            ((S).bkts[xuniq2(hash, xset_set_no_rehash)])                      \
                [xalen((S).bkts[xuniq2(hash, xset_set_no_rehash)]) - 1] = K;  \
        }                                                                     \
    } while (0)

// set key and value in set
#define xset_set(S, HASH, EQ, K)                                              \
    do {                                                                      \
        /* grow array if needed */                                            \
        if ((S).pop == xacap((S).bkts) / 2) {                                 \
            (S).len = (S).pop                = 0;                             \
            size_t xuniq2(old_cap, xset_set) = xacap((S).bkts);               \
            size_t xuniq2(new_cap, xset_set) = xacap((S).bkts) * 2;           \
                                                                              \
            /* double number of buckets and allocate 3rd region to            \
               temporarily store old data while rehashing */                  \
            xarr_resize((S).bkts, xuniq2(new_cap, xset_set) +                 \
                                      xuniq2(old_cap, xset_set));             \
                                                                              \
            /* move old data to 3rd region */                                 \
            memcpy((S).bkts + xuniq2(new_cap, xset_set), (S).bkts,            \
                   (xuniq2(old_cap, xset_set)) * sizeof((S).bkts[0]));        \
            memset((S).bkts, 0x0,                                             \
                   xuniq2(old_cap, xset_set) * sizeof((S).bkts[0]));          \
                                                                              \
            /* init new buckets */                                            \
            for (size_t xuniq2(bkt, xset_set) = 0;                            \
                 xuniq2(bkt, xset_set) < xuniq2(new_cap, xset_set);           \
                 ++xuniq2(bkt, xset_set)) {                                   \
                xarr_new_reserve((S).bkts[xuniq2(bkt, xset_set)], 2);         \
            }                                                                 \
                                                                              \
            /* rehash */                                                      \
            for (size_t xuniq2(bkt, xset_set) = 0;                            \
                 xuniq2(bkt, xset_set) < xuniq2(old_cap, xset_set);           \
                 ++xuniq2(bkt, xset_set)) {                                   \
                for (size_t xuniq2(i, xset_set) = 0;                          \
                     xuniq2(i, xset_set) <                                    \
                     xalen(                                                   \
                         ((S).bkts +                                          \
                          xuniq2(new_cap, xset_set))[xuniq2(bkt, xset_set)]); \
                     ++xuniq2(i, xset_set)) {                                 \
                    _xset_set_no_rehash(                                      \
                        S, xuniq2(new_cap, xset_set), HASH, EQ,               \
                        ((S).bkts + xuniq2(new_cap, xset_set))[xuniq2(        \
                            bkt, xset_set)][xuniq2(i, xset_set)]);            \
                }                                                             \
                                                                              \
                /* free old bucket after rehash  */                           \
                xarr_free(                                                    \
                    ((S).bkts +                                               \
                     xuniq2(new_cap, xset_set))[xuniq2(bkt, xset_set)]);      \
            }                                                                 \
                                                                              \
            /* shrink array to new capacity */                                \
            xacap((S).bkts) = xuniq2(new_cap, xset_set);                      \
        }                                                                     \
                                                                              \
        /* insert new value */                                                \
        _xset_set_no_rehash(S, xacap((S).bkts), HASH, EQ, K);                 \
    } while (0)

// remove key from set
#define xset_remove(S, HASH, EQ, K)                                           \
    do {                                                                      \
        size_t xuniq2(hash, xset_remove) = HASH(&(K)) % xacap((S).bkts);      \
                                                                              \
        for (size_t xuniq2(i, xset_remove) = 0;                               \
             xuniq2(i, xset_remove) <                                         \
             xalen((S).bkts[xuniq2(hash, xset_remove)]);                      \
             ++xuniq2(i, xset_remove)) {                                      \
            if (EQ(&(S).bkts[xuniq2(hash, xset_remove)]                       \
                            [xuniq2(i, xset_remove)],                         \
                   &(K))) {                                                   \
                xarr_remove((S).bkts[xuniq2(hash, xset_remove)],              \
                            xuniq2(i, xset_remove));                          \
                                                                              \
                (S).len--;                                                    \
                                                                              \
                if (xalen((S).bkts[xuniq2(hash, xset_remove)]) == 0) {        \
                    (S).pop--;                                                \
                }                                                             \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

#endif
