#include "../tokens/token.hpp"

namespace lexer {
std::ostream &operator<<(std::ostream &os, const lexer::Token &token) {
  os << token.line << "\t" << token.column << "\t" << token.class_name << "\t"
     << token.value << "\t" << token.raw_value;
  return os;
}

bool Token::is_literal() {
  return TokenType::literal_beg < this->type &&
         this->type < TokenType::literal_end;
}

bool Token::is_operator() {
  return TokenType::operator_beg < this->type &&
         this->type < TokenType::operator_end;
}

bool Token::is_separator() {
  return TokenType::separator_beg < this->type &&
         this->type < TokenType::separator_end;
}

bool Token::is_keyword() {
  return TokenType::keywords_beg < this->type &&
         this->type < TokenType::keywords_end;
}
} // namespace lexer
