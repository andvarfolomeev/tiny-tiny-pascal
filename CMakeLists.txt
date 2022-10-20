cmake_minimum_required(VERSION 2.8)

project(compotec)

set(SOURCE_EXE main.cpp lexer/token.cpp lexer/scanner.cpp)

add_executable(main ${SOURCE_EXE})
