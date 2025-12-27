# xlib

![C99](https://img.shields.io/badge/C-C99-informational)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat&logo=cmake&logoColor=white)

Beautiful ISO C99 container library focused on true genericity

Check out the [docs](https://github.com/ludvigsandberg/xlib/wiki) for examples

## Build

CMake

```sh
add_subdirectory(path/to/xlib)
target_link_libraries(target PRIVATE xlib)
```

Tests

```sh
cmake -B build -S . -DBUILD_TESTS=ON
cmake --build build
ctest --output-on-failure
```
