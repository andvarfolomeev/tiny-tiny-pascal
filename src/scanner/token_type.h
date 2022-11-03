#ifndef LEXER_TOKEN_TYPE_H
#define LEXER_TOKEN_TYPE_H

#include <map>
#include <string>
#include <vector>

namespace scanner {

enum TokenType {
    eof,
    INVALID,
    LITERAL_INTEGER,
    LITERAL_DOUBLE,
    LITERAL_STRING,
    ID,
    KEYWORD,
    COMMENT,
    OPER,
    SEPERATOR,
};
std::ostream &operator<<(std::ostream &os, const TokenType &type);
}  // namespace scanner

#endif  // LEXER_TOKEN_TYPE_H