#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

#include <fstream>
#include <sstream>
#include <utility>

#include "token.h"

namespace lexer {
class Scanner {
  std::ifstream &input_stream;
  unsigned int last_line, last_column, current_line, current_column,
      column_after_new_line;
  std::string buffer;
  bool is_eof;

  Token prepare_token(TokenType type, const std::string &value,
                      const std::string &raw_value) const;
  static bool is_space(char c);
  char consume();
  char unconsume();
  char try_consume(char c);
  char try_consume(bool (*func)(char));
  char peek();
  char buffer_peek();
  Token scan_string_literal();
  static bool is_digit(char c, int numeral_system = 10);
  Token scan_number_literal(int numeral_system);
  void skip_block_comment();
  void skip_block_comment_1();
  void skip_line_comment();
  static bool is_start_of_identifier(char c);
  static bool is_remainig_of_identifier(char c);
  Token scan_identifier_or_keyword();
  std::string get_integer_value(std::string raw, int numeral_system) const;
  static std::string get_real_value(const std::string &raw);

public:
  explicit Scanner(std::ifstream &input_stream)
      : input_stream(input_stream), last_line(1), last_column(1),
        current_line(1), current_column(1), column_after_new_line(1),
        is_eof(false) {}

  Token next_token();
  [[nodiscard]] bool eof() const;
};

class ScannerException : public std::exception {
public:
  [[nodiscard]] const char *what() const noexcept override {
    return message.c_str();
  }

  [[maybe_unused]] explicit ScannerException(unsigned int current_line,
                                             unsigned int current_column,
                                             const std::string &message) {
    std::ostringstream string_stream;
    string_stream << "Line: " << current_line << "; Column: " << current_column
                  << "; " << message;
    this->message = string_stream.str();
  }

protected:
  std::string message;
};

class UnexpectedTokenException : ScannerException {
public:
  UnexpectedTokenException(unsigned int current_line, unsigned current_column)
      : ScannerException(current_line, current_column, "Unexpected token") {}
};

class StringExceedsLineException : ScannerException {
public:
  StringExceedsLineException(unsigned int current_line, unsigned current_column)
      : ScannerException(current_line, current_column, "String exceeds line") {}
};

class UnterminatedBlockCommentException : ScannerException {
public:
  UnterminatedBlockCommentException(unsigned int current_line,
                                    unsigned current_column)
      : ScannerException(current_line, current_column,
                         "Unterminated block comment") {}
};

class IntegerOverflowException : ScannerException {
public:
  IntegerOverflowException(unsigned int current_line, unsigned current_column)
      : ScannerException(current_line, current_column, "Integer overflow") {}
};
} // namespace lexer

#endif // LEXER_SCANNER_H
