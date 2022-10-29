#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <iostream>
#include <string>
#include <utility>

#include "token_type.h"
#include "token_value_types.h"

namespace lexer
{
class Token
{
    unsigned int line, column;
    TokenType type;
    TokenValue value;
    std::string raw_value; // the string being scanned

  public:
    Token(unsigned int line, unsigned int column, TokenType type, TokenValue value, std::string raw_value)
        : line(line), column(column), type(type), value(std::move(value)), raw_value(std::move(raw_value)){};

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    [[nodiscard]] std::string to_string() const;

    TokenType get_type();
};
} // namespace lexer

#endif // LEXER_TOKEN_H
