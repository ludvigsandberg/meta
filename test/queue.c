#include <assert.h>

#include <x/queue.h>

int main(int argc, char **argv) {
    xqueue(int) q;
    xqueue_new(q, 10);

    assert(xqueue_len(q) == 0);

    xqueue_push(q, (int){5});
    xqueue_push(q, (int){10});
    xqueue_push(q, (int){15});
    assert(xqueue_len(q) == 3);

    assert(xqueue_pop(q) == 5);
    assert(xqueue_len(q) == 2);

    assert(xqueue_pop(q) == 10);
    assert(xqueue_len(q) == 1);

    xqueue_push(q, (int){20});
    assert(xqueue_len(q) == 2);

    assert(xqueue_pop(q) == 15);
    assert(xqueue_pop(q) == 20);
    assert(xqueue_len(q) == 0);

    xqueue_free(q);

    return 0;
}