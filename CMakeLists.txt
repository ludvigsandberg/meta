cmake_minimum_required(VERSION 3.0)

project(Meta)

add_library(Meta INTERFACE)

target_include_directories(Meta INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
