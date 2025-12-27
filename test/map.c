#include <assert.h>

#include <x/map.h>

typedef unsigned long ulong;

ulong hash_ulong(const ulong *k) {
    return *k;
}

int eq_ulong(const ulong *a, const ulong *b) {
    return *a == *b;
}

int main(int argc, char **argv) {
    xmap(ulong, int) m;
    xmap_new(m);

    ulong k1 = 1, k2 = 2, k3 = 3;
    int v1 = 10, v2 = 20, v3 = 30;

    xmap_set(m, hash_ulong, eq_ulong, k1, v1);
    xmap_set(m, hash_ulong, eq_ulong, k2, v2);
    xmap_set(m, hash_ulong, eq_ulong, k3, v3);

    assert(m.len == 3);

    int *v = NULL;
    xmap_get(m, hash_ulong, eq_ulong, k1, v);
    assert(v != NULL && *v == 10);

    v = NULL;
    xmap_get(m, hash_ulong, eq_ulong, k2, v);
    assert(v != NULL && *v == 20);

    v = NULL;
    xmap_get(m, hash_ulong, eq_ulong, k3, v);
    assert(v != NULL && *v == 30);

    bool contains = false;
    xmap_contains(m, hash_ulong, eq_ulong, k1, contains);
    assert(contains);

    ulong k_nonexist = 999;
    v                = NULL;
    xmap_get_maybe_null(m, hash_ulong, eq_ulong, k_nonexist, v);
    assert(v == NULL);

    xmap_set(m, hash_ulong, eq_ulong, k1, 100);
    assert(m.len == 3);
    v = NULL;
    xmap_get(m, hash_ulong, eq_ulong, k1, v);
    assert(v != NULL && *v == 100);

    xmap_remove(m, hash_ulong, eq_ulong, k2);
    assert(m.len == 2);

    v = NULL;
    xmap_get_maybe_null(m, hash_ulong, eq_ulong, k2, v);
    assert(v == NULL);

    xmap_free(m);

    return 0;
}