# xlib

![C99](https://img.shields.io/badge/C-C99-informational)

Beautiful container library for ISO C99

## Build

CMake

```sh
add_subdirectory(path/to/xlib)
target_link_libraries(target PRIVATE xlib)
```

CMake FetchContent

```sh
include(FetchContent)

FetchContent_Declare(xlib GIT_REPOSITORY https://github.com/ludvigsandberg/xlib)
FetchContent_MakeAvailable(xlib)

target_link_libraries(target PRIVATE xlib)
```

## Examples

todo

## Tests

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --output-on-failure
```
