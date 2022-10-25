#include "token_type.hpp"

namespace lexer {
std::string to_string(TokenType type) {
  switch (type) {
  case eof:
    return "EOF";
  case Integer:
    return "Integer";
  case Real:
    return "Real";
  case String:
    return "String";
  case Id:
    return "Id";
  case Keyword:
    return "Keyword";
  case Comment:
    return "Comment";
  case Operation:
    return "Operation";
  case Seperator:
    return "Separator";
  case Invalid:
    return "Invalid";
  }
}
} // namespace lexer
