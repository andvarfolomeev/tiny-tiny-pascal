#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

#include <fstream>
#include <sstream>

#include "token.hpp"

namespace lexer {
class Scanner {
  std::ifstream &input_stream;
  unsigned int last_line = 1, last_column = 1;
  unsigned int current_line, current_column;
  std::string buffer;
  bool is_eof;

  static bool is_space(char c);
  char consume();
  void unconsume();
  char try_consume(char c);
  char try_consume(bool (*func)(char));
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

class ScannerException : public std::exception {
public:
  [[nodiscard]] const char *what() const noexcept override {
    return this->message.c_str();
  }

  [[maybe_unused]] explicit ScannerException(unsigned int current_line,
                                             unsigned int current_column,
                                             std::string message) { // NOLINT
    std::ostringstream string_stream;
    string_stream << "Line: " << current_line << "; Column: " << current_column
                  << "; " << message;
    this->message = string_stream.str();
  }

protected:
  std::string message;
};
} // namespace lexer

#endif // LEXER_SCANNER_H
