#ifndef XQUEUE_H
#define XQUEUE_H

#include <stddef.h>

#include <x/arr.h>

// very simple fifo queue
// not memory efficient
#define xqueue(T)                                                             \
    struct {                                                                  \
        xarr(T) elems;                                                        \
        size_t head;                                                          \
    }

#define xqueue_len(Q) (xalen((Q).elems) - (Q).head)

#define xqueue_new(Q, CAP)                                                    \
    do {                                                                      \
        xarr_new_reserve((Q).elems, CAP);                                     \
        (Q).head = 0;                                                         \
    } while (0)

#define xqueue_free(Q) xarr_free((Q).elems)

#define xqueue_push(Q, ELEM) xarr_append((Q).elems, ELEM)

// always make sure queue has atleast one element before pop
#define xqueue_pop(Q) (Q).elems[(Q).head++]

#endif
