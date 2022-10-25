#include <iomanip>
#include <sstream>

#include "token.h"

namespace lexer {
std::ostream &operator<<(std::ostream &os, const lexer::Token &token) {
  os << token.line << "\t" << token.column << "\t" << to_string(token.type)
     << "\t" << token.value << "\t" << token.raw_value;
  return os;
}

std::string Token::to_string() const {
  std::ostringstream ss;
  ss << (*this);
  return ss.str();
}
TokenType Token::get_type() { return this->type; }
} // namespace lexer
