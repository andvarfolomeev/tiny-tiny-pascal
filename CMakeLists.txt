cmake_minimum_required(VERSION 2.8)
project(tiny_tiny_pascal)

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if (flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if (flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif ()
        unset(flag_supported CACHE)
    endif ()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

set(CMAKE_CXX_STANDARD 20)

# deps
include(cmake/CPM.cmake)
CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        GIT_TAG v0.8.1
)
CPMAddPackage(
        NAME argparse
        GITHUB_REPOSITORY p-ranav/argparse
        GIT_TAG v2.9
)

set(SOURCE_EXE
        src/scanner/buffered_istream.cpp
        src/scanner/scanner.cpp
        src/scanner/token.cpp
        src/scanner/token_type.cpp
        src/scanner/token_value_types.cpp
        src/pascal_types.h
        )

add_executable(tiny_tiny_pascal
        src/main.cpp
        ${SOURCE_EXE})
add_executable(tiny_tiny_pascal_tests
        src/simple_parser/simple_parser.cpp
        tests/tests.cpp
        tests/lexer.cpp
        tests/simple_parser.cpp
        tests/test_report.cpp
        ${SOURCE_EXE})

target_link_libraries(tiny_tiny_pascal PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal PRIVATE argparse)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE argparse)
