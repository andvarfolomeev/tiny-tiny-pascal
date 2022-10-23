#ifndef LEXER_TOKEN_TYPE_H
#define LEXER_TOKEN_TYPE_H

#include <map>
#include <string>

#include "../libs/enum.h"

namespace lexer {
BETTER_ENUM(   // NOLINT
    TokenType, // NOLINT
    char, INVALID = 1, eof, COMMENT,

    ID,
    INTEGER, FLOAT, CHAR, STRING,

    ADD, ADDASSIGN,

    SUB, SUBASSIGN,

    MUL, MULASSIGN,

    QUO, QUOASSIGN,

    EQL,

    LES, NEQ, LEQ,

    GTR, GEQ,

    ASSIGN,

    DEREF,

    AND, OR,

    NOT,

    XOR, SHL, SHR,

    IDIV, MDIV,

    INCLUDE, EXCLUDE, IN,

    IS, AS,

    LPAREN, RPAREN, LBRACK, RBRACK, COMMA, PERIOD, ELLIPSIS,
    SEMICOLON, COLON,

    BREAK, CASE, CONST, CONTINUE, ELSE, FOR, FUNCTION, PROCEDURE, GOTO, IF)

[[maybe_unused]] const std::map<std::string, TokenType> tokens = {
    {"INVALID", TokenType::INVALID},
    {"eof", TokenType::eof},
    {"COMMENT", TokenType::COMMENT},

    {"ID", TokenType::ID},
    // literals
    {"Integer", TokenType::INTEGER},
    {"Float", TokenType::FLOAT},
    {"Char", TokenType::CHAR},
    {"String", TokenType::STRING},

    // operators
    {"+", TokenType::ADD},
    {"+=", TokenType::ADDASSIGN},

    {"-", TokenType::SUB},
    {"-=", TokenType::SUBASSIGN},

    {"*", TokenType::MUL},
    {"*=", TokenType::MULASSIGN},

    {"/", TokenType::QUO},
    {"/=", TokenType::QUOASSIGN},

    {"=", TokenType::EQL},

    {"<", TokenType::LES},
    {"<>", TokenType::NEQ},
    {"<=", TokenType::LEQ},

    {">", TokenType::GTR},
    {">=", TokenType::GEQ},

    {":=", TokenType::ASSIGN},

    {"^", TokenType::DEREF},

    {"and", TokenType::AND},
    {"or", TokenType::OR},

    {"not", TokenType::NOT},

    {"xor", TokenType::XOR},
    {"shl", TokenType::SHL},
    {"shr", TokenType::SHR},

    {"div", TokenType::IDIV},
    {"mod", TokenType::MDIV},

    {"include", TokenType::INCLUDE},
    {"exclude", TokenType::EXCLUDE},
    {"in", TokenType::IN},

    {"is", TokenType::IS},
    {"as", TokenType::AS},

    // separators
    {"(", TokenType::LPAREN},
    {")", TokenType::RPAREN},
    {"[", TokenType::LBRACK},
    {"]", TokenType::RBRACK},
    {",", TokenType::COMMA},
    {".", TokenType::PERIOD},
    {"..", TokenType::ELLIPSIS},
    {";", TokenType::SEMICOLON},
    {":", TokenType::COLON},

    // keywords
    {"break", TokenType::BREAK},
    {"case", TokenType::CASE},
    {"const", TokenType::CONST},
    {"continue", TokenType::CONTINUE},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"procedure", TokenType::PROCEDURE},
    {"goto", TokenType::GOTO},
    {"if", TokenType::IF},
};
} // namespace lexer

#endif // LEXER_TOKEN_TYPE_H
