#include <cassert>
#include <iostream>

#include "scanner.hpp"
#include "tokens/token.hpp"

namespace lexer {

Token Scanner::next_token() { // NOLINT(misc-no-recursion)

  while (this->is_space(this->peek())) {
    this->consume();
  }

  TokenType token_type = TokenType::INVALID;

  this->buffer.clear();
  switch (this->consume()) {
  case EOF:
    token_type = TokenType::eof;
    this->is_eof = true;
    break;
  case '+':
    if (try_consume('=')) {
      token_type = TokenType::ADDASSIGN;
    } else {
      token_type = TokenType::ADD;
    }
    break;
  case '-':
    if (try_consume('=')) {
      token_type = TokenType::SUBASSIGN;
    } else {
      token_type = TokenType::SUB;
    }
    break;
  case '*':
    if (try_consume('=')) {
      token_type = TokenType::MULASSIGN;
    } else {
      token_type = TokenType::MUL;
    }
    break;
  case '/':
    if (try_consume('=')) {
      token_type = TokenType::QUOASSIGN;
    } else if (try_consume('*')) {
      this->skip_block_comment();
      return this->next_token();
    } else if (try_consume('/')) {
      this->skip_line_comment();
      ++this->current_line;
      this->current_column = 1;
      return this->next_token();
    } else {
      token_type = TokenType::QUO;
    }
    break;
  case '=':
    token_type = TokenType::EQL;
    break;
  case '<':
    if (try_consume('>')) {
      token_type = TokenType::NEQ;
    } else if (try_consume('=')) {
      token_type = TokenType::LEQ;
    } else {
      token_type = TokenType::LES;
    }
    break;
  case '>':
    if (try_consume('=')) {
      token_type = TokenType::GEQ;
    } else {
      token_type = TokenType::GTR;
    }
    break;
  case ':':
    if (try_consume('=')) {
      token_type = TokenType::ASSIGN;
    } else {
      token_type = TokenType::COLON;
    }
    break;
  case '^':
    token_type = TokenType::DEREF;
    break;
  case '(':
    token_type = TokenType::LPAREN;
    break;
  case ')':
    token_type = TokenType::RPAREN;
    break;
  case '[':
    token_type = TokenType::LBRACK;
    break;
  case ']':
    token_type = TokenType::RBRACK;
    break;
  case '{':
    token_type = TokenType::LBRACE;
    break;
  case '}':
    token_type = TokenType::RBRACE;
    break;
  case ',':
    token_type = TokenType::COMMA;
    break;
  case '.':
    if (try_consume('.')) {
      token_type = TokenType::ELLIPSIS;
    } else {
      token_type = TokenType::PERIOD;
    }
    break;
  case ';':
    token_type = TokenType::SEMICOLON;
    break;
  case '\'':
    this->scan_string_literal();
    token_type = TokenType::STRING;
    break;
  case '$':
    this->scan_number_literal(16);
    token_type = TokenType::INTEGER;
    break;
  case '&':
    this->scan_number_literal(8);
    token_type = TokenType::INTEGER;
    break;
  case '%':
    this->scan_number_literal(2);
    token_type = TokenType::INTEGER;
    break;
  default:
    if (is_digit(this->buffer_peek())) {
      this->scan_number_literal(10);
      token_type = TokenType::INTEGER;
    }
    if (is_start_of_identifier(this->buffer_peek())) {
      token_type = this->scan_identifier_or_keyword();
    }
    break;
  }

  std::string from_buffer = this->buffer;
  this->buffer.clear();

  if (token_type._value == TokenType::eof) {
    from_buffer = "EOF";
    return {this->current_line, this->current_column - 1, token_type,
            from_buffer, from_buffer};
  }

  return {this->current_line,
          static_cast<unsigned int>(this->current_column - from_buffer.size()),
          token_type, from_buffer, from_buffer};
}

bool Scanner::is_space(char c) {
  if (c == '\n') {
    ++this->current_line;
    this->current_column = 0;
  }
  return c == '\t' || c == ' ' || c == '\n';
}

char Scanner::consume() {
  char c = (char)this->input_stream.get();
  buffer.push_back(c);
  ++current_column;
  return c;
}

void Scanner::unconsume() {
  this->input_stream.unget();
  if (!this->buffer.empty()) {
    this->buffer.pop_back();
  } else {
    // TODO:
    assert(true);
  }
}

char Scanner::try_consume(char c) {
  if (this->input_stream.peek() == c) {
    this->consume();
    return true;
  }
  return false;
}

char Scanner::try_consume(bool (*func)(char)) {
  if (func((char)this->input_stream.peek())) {
    this->consume();
    return true;
  }
  return false;
}

char Scanner::peek() { return (char)this->input_stream.peek(); }

char Scanner::buffer_peek() {
  assert(!this->buffer.empty());
  return this->buffer[this->buffer.size() - 1];
}

void Scanner::scan_string_literal() {
  do {
    this->consume();
  } while (this->buffer_peek() != '\'');
}

bool Scanner::is_digit(char c) { return '0' <= c && c <= '9'; }

void Scanner::scan_number_literal(int numeral_system) {
  enum state {
    number = 1,
    number_after_dot,
    number_after_e,
    hex_number,
    octa_number,
    bin_number,
    finish
  };

  state current_state;
  char c;

  switch (numeral_system) {
  case 10:
    current_state = state::number;
    break;
  case 16:
    current_state = state::hex_number;
    break;
  case 8:
    current_state = state::octa_number;
    break;
  case 2:
    current_state = state::bin_number;
    break;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
  default:
    current_state = state::finish;
#pragma clang diagnostic pop
    assert(true);
  }

  while (current_state != finish) {
    c = this->consume();

    switch (current_state) {
    case number:
      if (is_digit(c)) {
        // consume
      } else if (c == '.' && this->try_consume(is_digit)) { // check next
        current_state = state::number_after_dot;
      } else if (c == 'e' && this->try_consume(is_digit)) {
        current_state = state::number_after_e;
      } else {
        this->unconsume(); // give back .
        current_state = finish;
      }
      break;
    case number_after_dot:
      if (!is_digit(c)) {
        if (c == 'e') {
          current_state = number_after_dot;
        } else {
          current_state = finish;
        }
      }
      break;
    case number_after_e:
      if (!is_digit(c)) {
        unconsume();
        current_state = finish;
      }
      break;
    case hex_number:
      if (!('0' <= c && c <= '9' && 'a' <= tolower(c) && tolower(c) <= 'z')) {
        unconsume();
        current_state = finish;
      }
      break;
    case octa_number:
      if (!('0' <= c && c <= '7')) {
        unconsume();
        current_state = finish;
      }
      break;
    case bin_number:
      if (!('0' <= c && c <= '1')) {
        unconsume();
        current_state = finish;
      }
      break;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
    case finish:
      break;
#pragma clang diagnostic pop
    }
  }
}

void Scanner::skip_block_comment() {
  for (;;) {
    // TODO: unterminated block comment exception
    this->consume();
    if (this->buffer_peek() == '*' && this->try_consume('/')) {
      return;
    }
  }
}

void Scanner::skip_line_comment() {
  for (;;) {
    this->consume();
    if (this->buffer_peek() == '\n') {
      return;
    }
  }
}

bool Scanner::is_start_of_identifier(char c) {
  return 'a' <= c && c <= 'z' || c == '_';
}

bool Scanner::is_remainig_of_identifier(char c) {
  return is_start_of_identifier(c) || is_digit(c);
}

TokenType Scanner::scan_identifier_or_keyword() {
  while (is_remainig_of_identifier(this->peek())) {
    this->consume();
  }
  auto result = TokenType::ID;
  if (tokens.find(this->buffer) != tokens.end()) {
    result = tokens.at(this->buffer);
  }
  return result;
}

bool Scanner::eof() const { return this->is_eof; }
} // namespace lexer
