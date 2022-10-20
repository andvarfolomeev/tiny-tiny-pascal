#include <iostream>
#include <stdio.h>

#include "scanner.hpp"

namespace lexer {
void Scanner::start() {
    char c;
    do {
        c = get_char();

        while (is_space(c)) {
          c = get_char();
        }

        switch (c) {
          case '\n':
            ++current_line;
            break;
          case '(':
            break;
          case ')':
            break;
          case '[':
            break;
          case ']':
            break;
          case '{':
            break;
          case '}':
            break;
          case ',':
            break;
          case '.':
            break;
          case ';':
            break;
          case ':':
            break;
        }

        std::cout << c << std::endl;
    } while(!this->input_stream.eof());
}

bool Scanner::is_space(char c) {
  return c == '\t' || c == ' ';
}
char Scanner::get_char() {
   return this->input_stream.get();
}
}
