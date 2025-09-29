cmake_minimum_required(VERSION 3.21)

project(Meta LANGUAGES C)

add_library(meta INTERFACE)

set_target_properties(meta PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_include_directories(meta INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
