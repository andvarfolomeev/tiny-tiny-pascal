cmake_minimum_required(VERSION 2.8)

project(tiny-tiny-pascal)

set(SOURCE_EXE main.cpp lexer/scanner.cpp lexer/tokens/token.cpp tests/lexer.cpp tests/lexer.h)
set(CMAKE_CXX_STANDARD 17)

add_executable(main ${SOURCE_EXE})
