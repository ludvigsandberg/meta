cmake_minimum_required(VERSION 3.21)
project(libx LANGUAGES C)

# library

add_library(libx INTERFACE)

set_target_properties(libx PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

target_include_directories(libx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
)

# tests

option(BUILD_TESTS "build tests" OFF)

if(BUILD_TESTS)
    enable_testing()

    file(GLOB TEST_SOURCES test/*.c)
    
    foreach(src ${TEST_SOURCES})
        get_filename_component(name ${src} NAME_WE)

        add_executable(${name} ${src})
        target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
        
        add_test(NAME ${name} COMMAND ${name})
    endforeach()
endif()


