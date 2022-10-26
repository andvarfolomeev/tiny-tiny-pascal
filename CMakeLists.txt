cmake_minimum_required(VERSION 2.8)

project(tiny-tiny-pascal)

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

set(SOURCE_EXE
        main.cpp
        lexer/scanner.cpp
        lexer/token.cpp
        lexer/token_type.cpp

        tests/lexer.cpp
        tests/test_report.cpp tests/test_report.h types.h)
set(CMAKE_CXX_STANDARD 17)

add_executable(main ${SOURCE_EXE})
