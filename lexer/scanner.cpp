#include <algorithm>
#include <cassert>

#include "scanner.h"
#include "token.h"
#include "token_type.h"

namespace lexer {

Token Scanner::next_token() { // NOLINT(misc-no-recursion)
  while (lexer::Scanner::is_space(this->peek())) {
    this->consume();
  }
  this->last_column = this->current_column;
  this->last_line = this->current_line;

  this->buffer.clear();
  switch (this->consume()) {
  case EOF:
    this->is_eof = true;
    return {this->last_line, this->last_column, TokenType::eof, "EOF", "EOF"};
  case '+':
  case '-':
  case '*':
    if (try_consume('=')) {
      goto prepare_token;
    } else {
      goto prepare_token;
    }
  case '/':
    if (try_consume('=')) { // NOLINT
      goto prepare_token;
    } else if (try_consume('/')) {
      this->skip_line_comment();
      return this->next_token();
    } else {
      goto prepare_token;
    }
  case '<':
    if (try_consume('>')) { // NOLINT
      goto prepare_token;
    } else if (try_consume('=')) {
      goto prepare_token;
    } else {
      goto prepare_token;
    }
  case ':':
  case '>':
    if (try_consume('=')) {
      goto prepare_token;
    } else {
      goto prepare_token;
    }
  case '=':
  case '^':
  case '(':
  case ')':
  case '[':
  case ']':
  case ',':
  case ';':
    goto prepare_token;
  case '{':
    this->skip_block_comment();
    return this->next_token();
  case '.':
    if (try_consume('.')) {
      goto prepare_token;
    } else {
      goto prepare_token;
    }
  case '\'':
    return this->scan_string_literal();
  case '$':
    return this->scan_number_literal(16);
  case '&':
    return this->scan_number_literal(8);
  case '%':
    return this->scan_number_literal(2);
  default:
    if (is_digit(this->buffer_peek())) {
      return this->scan_number_literal(10);
    }
    if (is_start_of_identifier(this->buffer_peek())) {
      return this->scan_identifier_or_keyword();
    }
    break;
  }
  if (token_type.find(buffer) == token_type.end() ||
      token_value.find(buffer) == token_value.end()) {
    return {this->last_line, this->last_column, TokenType::Invalid, buffer,
            buffer};
  }

prepare_token:
  return {this->last_line, this->last_column, token_type.at(buffer),
          token_value.at(buffer), buffer};
}

bool Scanner::is_space(char c) { return c == '\t' || c == ' ' || c == '\n'; }

/*
 * move to next character in stream and save character to buffer
 */
char Scanner::consume() {
  char c = (char)this->input_stream.get();
  buffer.push_back(c);
  if (c == '\n') {
    ++this->current_line;
    this->current_column = 1;
  } else {
    ++this->current_column;
  }
  return c;
}

/*
 * move to previous character in stream and delete last character from buffer
 */
void Scanner::unconsume() {
  this->input_stream.unget();
  if (!this->buffer.empty()) {
    if (this->buffer_peek() != '\n') {
      --this->current_column;
    }
    // TODO: restore current_line after unconsume '\n'
    this->buffer.pop_back();
  } else {
    // TODO: is this block of code reachable?
    assert(true);
  }
}

/*
 * move to the next character in the stream if that character is equal to the
 * character in the argument
 */
char Scanner::try_consume(char c) {
  if (this->input_stream.peek() == c) {
    this->consume();
    return true;
  }
  return false;
}

/*
 * move to the next character in the stream if that character is equal to the
 * character in the argument
 */
char Scanner::try_consume(bool (*func)(char)) {
  if (func((char)this->input_stream.peek())) {
    this->consume();
    return true;
  }
  return false;
}

/*
 * get the next character from the stream without move to it
 */
char Scanner::peek() { return (char)this->input_stream.peek(); }

/*
 * get the last character from buffer
 */
char Scanner::buffer_peek() {
  assert(!this->buffer.empty());
  return this->buffer[this->buffer.size() - 1];
}

Token Scanner::scan_string_literal() {
  do {
    this->consume();
    if (this->buffer_peek() == '\n' || this->buffer_peek() == EOF) {
      throw ScannerException(this->last_line, this->last_column,
                             "String exceeds line");
    }
  } while (this->buffer_peek() != '\'');
  return {this->last_line, this->last_column, TokenType::String, buffer,
          buffer};
}

bool Scanner::is_digit(char c) { return '0' <= c && c <= '9'; }

Token Scanner::scan_number_literal(int numeral_system) {
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

  auto type = TokenType::Integer;

  switch (numeral_system) {
  case 16:
    current_state = state::hex_number;
    break;
  case 8:
    current_state = state::octa_number;
    break;
  case 2:
    current_state = state::bin_number;
    break;
  default:
    assert(numeral_system == 10);
    current_state = state::number;
  }

  while (current_state != finish) {
    c = this->consume();

    switch (current_state) {
    case number:
      if (is_digit(c)) {
        // consume
      } else if (c == '.' && this->try_consume(is_digit)) { // check next
        current_state = state::number_after_dot;
        type = TokenType::Real;
      } else if (c == 'e' && this->try_consume(is_digit)) {
        current_state = state::number_after_e;
        type = TokenType::Real;
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
          this->unconsume(); // give back
          current_state = finish;
        }
      }
      break;
    case number_after_e:
      if (!is_digit(c)) {
        unconsume(); // give back
        current_state = finish;
      }
      break;
    case hex_number:
      if (!('0' <= c && c <= '9' && 'a' <= tolower(c) && tolower(c) <= 'z')) {
        unconsume(); // give back
        current_state = finish;
      }
      break;
    case octa_number:
      if (!('0' <= c && c <= '7')) {
        unconsume(); // give back
        current_state = finish;
      }
      break;
    case bin_number:
      if (!('0' <= c && c <= '1')) {
        unconsume(); // give back
        current_state = finish;
      }
      break;
    default:
      break;
    }
  }
  return {this->last_line, this->last_column, type, buffer, buffer};
}

void Scanner::skip_block_comment() {
  for (;;) {
    this->consume();
    if (this->buffer_peek() == '}') {
      return;
    } else if (this->buffer_peek() == EOF) {
      throw ScannerException(this->last_line, this->last_column,
                             "Unterminated block comment");
    }
  }
}

void Scanner::skip_line_comment() {
  for (;;) {
    this->consume();
    if (this->buffer_peek() == '\n' || this->buffer_peek() == EOF) {
      return;
    }
  }
}

bool Scanner::is_start_of_identifier(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool Scanner::is_remainig_of_identifier(char c) {
  return is_start_of_identifier(c) || is_digit(c);
}

Token Scanner::scan_identifier_or_keyword() {
  while (is_remainig_of_identifier(this->peek())) {
    this->consume();
  }
  auto type = TokenType::Id;

  std::string buffer_in_lower = this->buffer;
  std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                 buffer_in_lower.begin(), ::tolower);

  if (std::find(keywords.begin(), keywords.end(), buffer_in_lower) !=
      keywords.end()) {
    type = TokenType::Keyword;
  }

  return {this->last_line, this->last_column, type, buffer_in_lower, buffer};
}

bool Scanner::eof() const { return this->is_eof; }
} // namespace lexer
