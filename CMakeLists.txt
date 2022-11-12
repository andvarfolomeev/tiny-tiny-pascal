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

set(
        SOURCE

        src/scanner/buffered_istream.cpp
        src/scanner/buffered_istream.h

        src/scanner/scanner.cpp
        src/scanner/scanner.h

        src/scanner/token.cpp
        src/scanner/token.h

        src/simple_parser/node.cpp
        src/simple_parser/node.h
        src/simple_parser/simple_parser.cpp
        src/simple_parser/simple_parser.h

        src/pascal_types.h
)

set(
        SOURCE_MAIN_EXE
        ${SOURCE}
        src/main.cpp
)

set(
        SOURCE_TEST_EXE
        ${SOURCE}
        testers/test_report.cpp
        testers/basic_tester.cpp
        testers/basic_tester.h
        testers/scanner_tester.cpp
        testers/scanner_tester.h
        testers/simple_parser_tester.cpp
        testers/simple_parser_tester.h
        testers/tests.cpp
)

add_executable(tiny_tiny_pascal ${SOURCE_MAIN_EXE})
add_executable(tiny_tiny_pascal_tests ${SOURCE_TEST_EXE})

target_link_libraries(tiny_tiny_pascal PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal PRIVATE argparse)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE argparse)
