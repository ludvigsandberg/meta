#ifndef XQUEUE_H
#define XQUEUE_H

#include <stddef.h>

#include <x/arr.h>

// very simple fifo queue
// not memory efficient
#define queue(T)                                                              \
    struct {                                                                  \
        arr(T) elems;                                                         \
        size_t head;                                                          \
    }

#define queue_len(Q) (alen((Q).elems) - (Q).head)

#define queue_new(Q, CAP)                                                     \
    do {                                                                      \
        arr_new_reserve((Q).elems, CAP);                                      \
        (Q).head = 0;                                                         \
    } while (0)

#define queue_free(Q) arr_free((Q).elems)

#define queue_push(Q, ELEM) arr_append((Q).elems, ELEM)

// always make sure queue has atleast one element before pop
#define queue_pop(Q) (Q).elems[(Q).head++]

#endif
