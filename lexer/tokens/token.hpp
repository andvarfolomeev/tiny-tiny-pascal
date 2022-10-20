#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <iostream>
#include <string>
#include "../tokens/token_type.hpp"

namespace lexer {
class Token {
  unsigned int line;
  unsigned int column;

  TokenType type;
  std::string class_name;
  std::string value;
  std::string raw_value; // the string being scanned

public:
  Token(int line, int column, std::string class_name, std::string value,
        std::string raw_value)
      : line(line), column(column), class_name(class_name), value(value),
        raw_value(raw_value){};

  friend std::ostream &operator<<(std::ostream &os, const Token &token);

  bool is_literal();
  bool is_operator();
  bool is_separator();
  bool is_keyword();
};
} // namespace lexer

#endif // LEXER_TOKEN_H
