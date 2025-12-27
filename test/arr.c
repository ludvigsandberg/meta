#include <assert.h>

#include <x/arr.h>

int main(int argc, char **argv) {
    int *arr = NULL;
    xarr_new(arr);
    assert(xalen(arr) == 0);
    assert(xacap(arr) >= 0);
    xarr_free(arr);

    xarr_new_n(arr, 5);
    assert(xalen(arr) == 5);
    assert(xacap(arr) == 5);
    xarr_free(arr);

    xarr_new_n_zero(arr, 3);
    assert(xalen(arr) == 3);
    assert(xacap(arr) == 3);
    assert(arr[0] == 0 && arr[1] == 0 && arr[2] == 0);
    xarr_free(arr);

    xarr_new_reserve(arr, 10);
    assert(xalen(arr) == 0);
    assert(xacap(arr) == 10);
    xarr_free(arr);

    xarr_new(arr);
    int val1 = 42, val2 = 100;
    xarr_append(arr, val1);
    assert(xalen(arr) == 1);
    assert(arr[0] == 42);

    xarr_append(arr, val2);
    assert(xalen(arr) == 2);
    assert(arr[1] == 100);
    xarr_free(arr);

    xarr_new(arr);
    xarr_append(arr, (int){10});
    xarr_append(arr, (int){30});
    int val3 = 20;
    xarr_insert(arr, 1, val3);
    assert(xalen(arr) == 3);
    assert(arr[0] == 10 && arr[1] == 20 && arr[2] == 30);
    xarr_free(arr);

    xarr_new(arr);
    xarr_append(arr, (int){1});
    xarr_append(arr, (int){2});
    xarr_append(arr, (int){3});
    xarr_remove(arr, 1);
    assert(xalen(arr) == 2);
    assert(arr[0] == 1 && arr[1] == 3);
    xarr_free(arr);

    xarr_new_n(arr, 5);
    xarr_resize(arr, 10);
    assert(xalen(arr) == 10);
    assert(xacap(arr) == 10);
    xarr_free(arr);

    return 0;
}