cmake_minimum_required(VERSION 3.21)
project(xlib LANGUAGES C)

# library

add_library(xlib INTERFACE)

set_target_properties(xlib PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_include_directories(xlib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# tests

option(BUILD_TESTS "" OFF)

if(BUILD_TESTS)
    enable_testing()

    file(GLOB TEST_SOURCES test/*.c)
    
    foreach(src ${TEST_SOURCES})
        get_filename_component(test_name ${src} NAME_WE)
        add_executable(${test_name} ${src})
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()


