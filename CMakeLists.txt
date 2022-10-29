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

set(SOURCE_EXE
        src/main.cpp

        src/lexer/scanner.cpp
        src/lexer/token.cpp
        src/lexer/token_type.cpp
        src/lexer/buffered_istream.cpp
        src/lexer/buffered_istream.h
        src/lexer/token_value_types.cpp
        src/lexer/token_value_types.h

        src/pascal_types.h

        tests/lexer.cpp
        tests/test_report.cpp
        tests/test_report.h)

add_executable(tiny_tiny_pascal ${SOURCE_EXE})
target_link_libraries(tiny_tiny_pascal PRIVATE magic_enum)