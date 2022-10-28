#ifndef TOKEN_VALUE_TYPES_H
#define TOKEN_VALUE_TYPES_H

#include <string>
#include <variant>

namespace lexer {
typedef int Integer;
typedef double Double;
typedef std::string String;

enum Operators {
  ADD,       // +
  ADDASSIGN, // +=
  SUB,       // -
  SUBASSIGN, // -=
  MUL,       // *
  MULASSIGN, // *=
  QUO,       // /
  QUOASSIGN, // /=
  EQL,       // =
  LES,       // <
  NEQ,       // <>
  LEQ,       // <=
  GTR,       // >
  GEQ,       // >=
  ASSIGN,    // :=
  DEREF      // ^
};

enum Separators {
  LPAREN,    // (
  RPAREN,    // )
  LBRACK,    // [
  RBRACK,    // ]
  COMMA,     // ,
  PERIOD,    // .
  ELLIPSIS,  // ..
  SEMICOLON, // ;
  COLON      // :
};

enum Keywords {
  ABSOLUTE,
  ABSTRACT,
  ALIAS,
  AND,
  ARRAY,
  AS,
  ASM,
  ASSEMBLER,
  BEGIN,
  BREAK,
  CASE,
  CDECL,
  CLASS,
  CONST,
  CONSTREF,
  CONSTRUCTOR,
  CONTINUE,
  CPPDECL,
  DEFAULT,
  DESTRUCTOR,
  DISPOSE,
  DIV,
  DO,
  DOWNTO,
  ELSE,
  END,
  EXCEPT,
  EXIT,
  EXPORT,
  EXPORTS,
  EXTERNAL,
  FALSE,
  FILE,
  FINALIZATION,
  FINALLY,
  FOR,
  FORWARD,
  FUNCTION,
  GENERIC,
  GOTO,
  IF,
  IMPLEMENTATION,
  IN,
  INDEX,
  INHERITED,
  INITIALIZATION,
  INLINE,
  INTERFACE,
  IS,
  LABEL,
  LIBRARY,
  LOCAL,
  MOD,
  NAME,
  NEW,
  NIL,
  NOSTACKFRAME,
  NOT,
  OBJECT,
  OF,
  OLDFPCCALL,
  ON,
  OPERATOR,
  OR,
  OUT,
  OVERRIDE,
  PACKED,
  PASCAL,
  PRIVATE,
  PROCEDURE,
  PROGRAM,
  PROPERTY,
  PROTECTED,
  PUBLIC,
  PUBLISHED,
  RAISE,
  READ,
  RECORD,
  REGISTER,
  REINTRODUCE,
  REPEAT,
  SAFECALL,
  SELF,
  SET,
  SHL,
  SHR,
  SOFTFLOAT,
  SPECIALIZE,
  STDCALL,
  STRING,
  THEN,
  THREADVAR,
  TO,
  TRUE,
  TRY,
  TYPE,
  UNIT,
  UNTIL,
  USES,
  VAR,
  VIRTUAL,
  WHILE,
  WITH,
  WRITE,
  XOR,
};

std::ostream &operator<<(std::ostream &os, const Operators &op);
std::ostream &operator<<(std::ostream &os, const Separators &sep);
std::ostream &operator<<(std::ostream &os, const Keywords &keyword);

typedef std::variant<Integer, Double, String, Operators, Separators, Keywords>
    TokenValue;
} // namespace lexer

#endif // TOKEN_VALUE_TYPES_H
