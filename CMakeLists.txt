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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-g")
enable_cxx_compiler_flag_if_supported("-Wextra")
#enable_cxx_compiler_flag_if_supported("-pedantic")

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

        src/exception.cpp
        src/exception.h

        src/scanner/buffered_istream.cpp
        src/scanner/buffered_istream.h

        src/scanner/scanner.cpp
        src/scanner/scanner.h

        src/scanner/token.cpp
        src/scanner/token.h

        src/parser/parser.h
        src/parser/parser.cpp
        src/parser/node/node.h
        src/parser/node/node.cpp
        src/parser/node/node_declaration.h
        src/parser/node/node_declaration.cpp
        src/parser/node/node_base_expression.h
        src/parser/node/node_expression.h
        src/parser/node/node_expression.cpp
        src/parser/node/node_program.h
        src/parser/node/node_program.cpp
        src/parser/node/node_statement.h
        src/parser/node/node_statement.cpp
        src/parser/node/node_type.h
        src/parser/node/node_type.cpp

        src/symbol_table/symbol.h
        src/symbol_table/symbol.cpp
        src/symbol_table/symbol_function.h
        src/symbol_table/symbol_function.cpp
        src/symbol_table/symbol_table.h
        src/symbol_table/symbol_table.cpp
        src/symbol_table/symbol_table_stack.h
        src/symbol_table/symbol_table_stack.cpp
        src/symbol_table/symbol_type.h
        src/symbol_table/symbol_type.cpp
        src/symbol_table/symbol_type_array.h
        src/symbol_table/symbol_type_array.cpp
        src/symbol_table/symbol_var.h
        src/symbol_table/symbol_var.cpp

        src/visitor/base_visitor.h
        src/visitor/base_visitor.cpp
        src/visitor/semantic_visitor.h
        src/visitor/semantic_visitor.cpp
        src/visitor/printer_visitor.h
        src/visitor/printer_visitor.cpp

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
        tests/tester.cpp
        tests/tester.h
        tests/test_report.cpp
        tests/test_report.h
        tests/main.cpp
)

add_executable(tiny_tiny_pascal ${SOURCE_MAIN_EXE})
add_executable(tiny_tiny_pascal_tests ${SOURCE_TEST_EXE})

target_link_libraries(tiny_tiny_pascal PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal PRIVATE argparse)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE magic_enum)
target_link_libraries(tiny_tiny_pascal_tests PRIVATE argparse)
