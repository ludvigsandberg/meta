# x

![C99](https://img.shields.io/badge/C-C99-informational)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat&logo=cmake&logoColor=white)

Beautiful ISO C99 container library

Provides arrays, queues, hash maps, sets, vectors and matrices

See the [docs](https://github.com/ludvigsandberg/x/wiki) for examples

## Build

CMake

```sh
add_subdirectory(path/to/x)
target_link_libraries(target PRIVATE x)
```

Tests

```sh
cmake -B build -S . -DBUILD_TESTS=ON
cmake --build build
ctest --output-on-failure --test-dir build -C Debug
```
