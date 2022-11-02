#ifndef LEXER_TOKEN_TYPE_H
#define LEXER_TOKEN_TYPE_H

#include <map>
#include <string>
#include <vector>

namespace lexer {

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
}  // namespace lexer

#endif  // LEXER_TOKEN_TYPE_H