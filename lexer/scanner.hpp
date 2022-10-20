#include <fstream>

#include "tokens/token.hpp"

#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

namespace lexer {
class Scanner {
  std::ifstream &input_stream;
  unsigned int current_line, current_column;

public:
  Scanner(std::ifstream &input_stream)
      : current_line(1), current_column(0), input_stream(input_stream) {}
  void start();
  Token new_token();
  bool is_space(char c);
  char get_char();
  unsigned int get_current_line_number();
  unsigned int get_current_column_number();
};
} // namespace lexer

#endif // LEXER_SCANNER_H
