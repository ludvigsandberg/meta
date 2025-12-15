# meta.h

![C99](https://img.shields.io/badge/C-C99-informational)

Very lightweight container library for C99.
Single header, macro based and somewhat type safe.
Contains dynamic array, hash map and queue.

## Usage

The easiest way to use this library is to copy `meta.h` into your project. That's it.

Alternatively use CMake:

```sh
add_subdirectory(path/to/meta)
target_link_libraries(MyExecutable PRIVATE Meta)
```

Or CMake FetchContent:

```sh
include(FetchContent)

FetchContent_Declare(Meta GIT_REPOSITORY https://github.com/ludvigsandberg/meta)
FetchContent_MakeAvailable(Meta)

target_link_libraries(MyExecutable PRIVATE Meta)
```

## Includes

The project relies on a handful of standard library features but doesn't include anything by default.
The user decides which features to use and should include the appropriate standard library headers.
Common headers you may need: `stdlib.h`, `string.h`, `assert.h`, `stdint.h`, `stdbool.h`

## Notes on Type Safety

While it doesn't prevent you from mixing types in all cases,
it does prevent a lot of type related bugs if you have the conversion warnings enabled.
Data is stored as the type you pass when you declare the containers, never `void*`,
and it's up to the compiler and the warnings to catch type mismatchs.

## Examples

### Array

```c
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <meta.h>

int main() {
    arr(double) a;
    arr_new(a);

    // arr_new_reserve(a, 10);
    // arr_new_n(a, 10);
    // arr_new_n_zero(a, 10);

    alen(a); // length
    acap(a); // capacity

    double v = 4.;
    arr_append(a, v);

    double vs[5] = {1., 2., 3., 4., 5.};
    arr_append_n(a, 5, vs);

    arr_insert_n(a, 0, 5, vs);

    arr_remove(a, 0);
    arr_remove_n(a, 0, 5);

    arr_free(a);
}
```

### String

The array API is well suited for handling strings. Note that these strings are not null-terminated, so be mindful when interacting with standard library functions.

```c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <meta.h>

typedef arr(char) string_t;

int main() {
    string_t s;

    // string literal to array
    // zero memory allocations
    // works by concatenating string literal and length during preprocessing
    arr_from_string_literal(s, "hello world", 11, 0);

    printf("str=\"%.*s\" len=%zu\n", (int)alen(s), s, alen(s));
    // str="hello world" len=11

    // no need to free array since it's a string literal
}
```

### Hash Map

Hash maps are fully generic.

```c
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <meta.h>

typedef arr(char) string_t;

// typedef if passing hash map to functions
typedef map(string_t, float) string_to_float_map_t;

// user defined hash function
uint64_t hash(const string_t *key) {
    uint64_t hash = 14695981039346656037ul;
    for (size_t i = 0; i < alen(*key); i++) {
        hash ^= (uint64_t)(uint8_t)(*key)[i];
        hash *= 1099511628211ul;
    }
    return hash;
}

// user defined equality function
bool eq(const string_t *a, const string_t *b) {
    return alen(*a) == alen(*b) && memcmp(*a, *b, alen(*a)) == 0;
}

int main() {
    string_to_float_map_t m;
    map_new(m);

    string_t key;
    arr_from_string_literal(key, "hello", 5, 0);
    float val = 123.f;
    map_set(m, hash, eq, key, val);

    string_t *k;
    float *v;
    map_foreach(m, k, v) {
        printf("key=%.*s val=%f\n", (int)alen(*k), *k, *v);
    }
    // key=hello val=123.000000

    map_remove(m, hash, eq, key);
    printf("map length after remove=%d\n", (int)m.len);
    // map length after remove=0

    map_free(m);
}
```
