#ifndef LEXER_TOKEN_TYPE_H
#define LEXER_TOKEN_TYPE_H

#include <map>
#include <string>

namespace lexer {
enum TokenType {
  eof = 1,
  COMMENT,

  literal_beg,
  INTEGER, // 12345, -12345, $ABC, &567, %101
  FLOAT,   // 123.123
  CHAR,    // 'a'
  STRING,  // 'abc'
  literal_end,

  operator_beg,
  ADD, // +
  SUB, // -
  MUL, // *
  QUO, // /
  REM, // %

  AND, // and
  OR,  // or

  EQL, // =
  NEQ, // <>

  ASSIGN,    // :=
  ADDASSIGN, // +=
  SUBASSIGN, // -=
  MULASSIGN, // *=
  QUOASSIGN, // /=
  operator_end,

  separator_beg,
  LPAREN,    // (
  RPAREN,    // )
  LBRACK,    // [
  RBRACK,    // ]
  LBRACE,    // {
  RBRACE,    // }
  COMMA,     // ,
  PERIOD,    // .
  SEMICOLON, // ;
  COLON,     // :
  separator_end,

  keywords_beg,
  BREAK,
  CASE,
  CONST,
  CONTINUE,
  ELSE,
  FOR,
  FUNCTION,
  PROCEDURE,
  GOTO,
  IF,
  keywords_end,
};

const std::map<TokenType, std::string> tokens = {
    // literals
    {INTEGER, "Integer"},
    {FLOAT, "Float"},
    {CHAR, "Char"},
    {STRING, "String"},
    // operators
    {ADD, "+"},
    {SUB, "-"},
    {MUL, "*"},
    {QUO, "/"},
    {REM, "%"},
    {AND, "and"},
    {OR, "or"},
    {EQL, "="},
    {NEQ, "<>"},
    {ASSIGN, ":="},
    {ADDASSIGN, "+="},
    {SUBASSIGN, "-="},
    {MULASSIGN, "*="},
    {QUOASSIGN, "/="},
    // separators
    {LPAREN, "("},
    {RPAREN, ")"},
    {LBRACK, "["},
    {RBRACK, "]"},
    {LBRACE, "{"},
    {RBRACE, "}"},
    {COMMA, ","},
    {PERIOD, "."},
    {SEMICOLON, ";"},
    {COLON, ":"},
    // keywords
    {BREAK, "break"},
    {CASE, "case"},
    {CONST, "const"},
    {CONTINUE, "continue"},
    {ELSE, "else"},
    {FOR, "for"},
    {FUNCTION, "function"},
    {PROCEDURE, "procedure"},
    {GOTO, "goto"},
    {IF, "if"},
};
} // namespace lexer

#endif // LEXER_TOKEN_TYPE_H
