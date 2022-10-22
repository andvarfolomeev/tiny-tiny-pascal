#include <fstream>

#include "tokens/token.hpp"

#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

namespace lexer {
class Scanner {
  std::ifstream &input_stream;
  unsigned int current_line, current_column;
  std::string buffer;
  bool is_eof;

  bool is_space(char c);
  char consume();
  void unconsume();
  char try_consume(char c);
  char try_consume(bool (*func) (char));
  char peek();
  char buffer_peek();
  void scan_string_literal();
  static bool is_digit(char c);
  TokenType scan_number_literal(int numeral_system);
  void skip_block_comment();
  void skip_line_comment();
  static bool is_start_of_identifier(char c);
  static bool is_remainig_of_identifier(char c);
  TokenType scan_identifier_or_keyword();

public:
  explicit Scanner(std::ifstream &input_stream)
      : current_line(1), current_column(1), input_stream(input_stream),
        is_eof(false) {}

  Token next_token();
  [[nodiscard]] bool eof() const;
};
} // namespace lexer

#endif // LEXER_SCANNER_H
