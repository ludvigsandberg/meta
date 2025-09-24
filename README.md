# meta.h
Ultra-lightweight container library fully compatible with standard C99.
Single-header, macro-based, type-safe, and "include-what-you-need".
Contains implementations for arrays and hash maps.

## Usage
### 1. Simple copy
The easiest way to use this library is to copy `meta.h` into your project. That's it.

### 2. Using CMake
```sh
add_subdirectory(path/to/meta)
target_link_libraries(MyExecutable PRIVATE Meta)
```

### 3. Using CMake + FetchContent
```sh
include(FetchContent)

FetchContent_Declare(Meta GIT_REPOSITORY https://github.com/ludvigsandberg/meta)
FetchContent_MakeAvailable(Meta)

target_link_libraries(MyExecutable PRIVATE Meta)
```

## Include-What-You-Need
This project relies on a handful of standard library features but does not include anything by default.
The user decides which features of `meta.h` to use and must include the appropriate standard library headers.
This reduces compilation time.

Common headers you may need: `stdlib.h`, `string.h`, `assert.h`, `stdint.h`, `stdbool.h`

## Examples
### Arrays
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

    alen(a); // Length
    acap(a); // Capacity

    /* Append single value */
    double v = 4.;
    arr_append(a, v);

    /* Append array */
    double vs[5] = {1., 2., 3., 4., 5.};
    arr_append_n(a, 5, vs);

    /* Insert array */
    arr_insert_n(a, 0, 5, vs);

    /* Remove values */
    arr_remove(a, 0);
    arr_remove_n(a, 0, 5);

    arr_free(a);
}
```

### Strings
You can use the array API to handle strings. Note that these strings are not null-terminated, so care is needed when interacting with standard library functions.
```c
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <meta.h>

typedef arr(char) string_t;

int main() {
    string_t s;

    /* Create array from string literal with embedded metadata */
    arr_from_string_literal(s, "String literal with embedded metadata.", 38, 0);

    /* Print using %.*s with cast to int for alen(s) (size_t -> int) */
    printf("String=\"%.*s\"\n", (int)alen(s), s);
    printf("String len=%zu\n", alen(s));

    /* No need to free since it's a string literal */
}
```
#### Output:
```
String="String literal with embedded metadata."
String len=38
```

### Hash Maps
Hash maps in `meta.h` support any key and value types, including custom structs, by allowing you to provide your own hash and equality functions.
```c
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <meta.h>

typedef arr(char) string_t;

/* Type define hash map if passing into functions */
typedef map(string_t, float) string_to_float_map_t;

/* User-defined hash function */
uint64_t hash(const string_t *key) {
    uint64_t hash = 14695981039346656037ul;
    for (size_t i = 0; i < alen(*key); i++) {
        hash ^= (uint64_t)(uint8_t)(*key)[i];
        hash *= 1099511628211ul;
    }
    return hash;
}

/* User-defined equality function */
bool eq(const string_t *a, const string_t *b) {
    return alen(*a) == alen(*b) && memcmp(*a, *b, alen(*a)) == 0;
}

int main() {
    string_to_float_map_t m;
    map_new(m);

    /* Set key-value pair */
    string_t key;
    arr_from_string_literal(key, "hello", 5, 0);
    float val = 123.f;

    /* Pass hash and eq function explicitly */
    map_set(m, hash, eq, key, val);

    /* Iterate */
    string_t *k;
    float *v;
    map_foreach(m, k, v) {
        printf("Key=%.*s Val=%f\n", (int)alen(*k), *k, *v);
    }

    map_remove(m, hash, eq, key);
    printf("Map length after remove=%d\n", (int)m.len);

    map_free(m);
}
```
#### Output:
```
Key=hello Val=123.000000
Map length after remove=0
```
