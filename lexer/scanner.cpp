#include <algorithm>
#include <cassert>
#include <cmath>

#include "../types.h"
#include "scanner.h"
#include "token.h"
#include "token_type.h"

namespace lexer {

Token Scanner::next_token() {
  char c;

  do {
    c = consume();

    // save position before call skip_*_comment
    // for correct position in exception
    last_column = current_column - 1;
    last_line = current_line;

    if (c == '/' && try_consume('/')) {
      skip_line_comment();
    } else if (c == '{') {
      skip_block_comment();
    } else if (c == '(' && try_consume('*')) {
      skip_block_comment_1();
    } else if (!is_space(c)) {
      break;
    }
  } while (true);

  // take into account that the consume method was called
  last_column = current_column - 1;
  last_line = current_line;
  buffer.clear();
  buffer.push_back(c);

  switch (c) {
  case EOF:
    is_eof = true;
    return prepare_token(TokenType::eof, "EOF", "EOF");
  case '+':
  case '-':
  case '*':
  case '/':
    if (try_consume('=')) {
      return prepare_token(token_type.at(buffer), token_value.at(buffer),
                           buffer);
    }
    return prepare_token(token_type.at(buffer), token_value.at(buffer), buffer);
  case '<':
    if (try_consume('>')) {
      return prepare_token(token_type.at(buffer), token_value.at(buffer),
                           buffer);
    } else if (try_consume('=')) {
      return prepare_token(token_type.at(buffer), token_value.at(buffer),
                           buffer);
    }
    return prepare_token(token_type.at(buffer), token_value.at(buffer), buffer);
  case ':':
  case '>':
    if (try_consume('=')) {
      return prepare_token(token_type.at(buffer), token_value.at(buffer),
                           buffer);
    }
    return prepare_token(token_type.at(buffer), token_value.at(buffer), buffer);
  case '=':
  case '^':
  case '(':
  case ')':
  case '[':
  case ']':
  case ',':
  case ';':
    return prepare_token(token_type.at(buffer), token_value.at(buffer), buffer);
  case '.':
    if (try_consume('.')) {
      return prepare_token(token_type.at(buffer), token_value.at(buffer),
                           buffer);
    }
    return prepare_token(token_type.at(buffer), token_value.at(buffer), buffer);
  case '\'':
    return scan_string_literal();
  case '$':
    if (try_consume([](char c) { return is_digit(c, 16); })) {
      return scan_number_literal(16);
    }
  case '&':
    if (try_consume([](char c) { return is_digit(c, 8); })) {
      return scan_number_literal(8);
    }
  case '%':
    if (try_consume([](char c) { return is_digit(c, 2); })) {
      return scan_number_literal(2);
    }
  default:
    if (is_digit(buffer_peek())) {
      return scan_number_literal(10);
    }
    if (is_start_of_identifier(buffer_peek())) {
      return scan_identifier_or_keyword();
    }
    break;
  }

  // means that the token type is invalid
  throw ScannerException(last_line, last_column, "Unexpected token");
}

bool Scanner::is_space(char c) { return c == '\t' || c == ' ' || c == '\n'; }

/*
 * move to next character in stream and save character to buffer
 */
char Scanner::consume() {
  char c = (char)input_stream.get();
  buffer.push_back(c);
  if (c == '\n') {
    column_after_new_line = current_line;
    ++current_line;
    current_column = 1;
  } else {
    ++current_column;
  }
  return c;
}

/*
 * move to the next character in the stream if that character is equal to the
 * character in the argument
 */
char Scanner::try_consume(char c) {
  if (input_stream.peek() == c) {
    consume();
    return true;
  }
  return false;
}

/*
 * move to the next character in the stream if that character is equal to the
 * character in the argument
 */
char Scanner::try_consume(bool (*func)(char)) {
  if (func((char)input_stream.peek())) {
    consume();
    return true;
  }
  return false;
}

/*
 * move to previous character in stream and delete last character from buffer
 */
char Scanner::unconsume() {
  input_stream.unget();
  if (!buffer.empty()) {
    if (buffer_peek() != '\n') {
      --current_column;
    } else {
      current_line--;
      current_column = column_after_new_line;
    }
    buffer.pop_back();
    return buffer.back();
  } else {
    assert(true);
  }
  return EOF;
}

/*
 * get the next character from the stream without move to it
 */
char Scanner::peek() { return (char)input_stream.peek(); }

/*
 * get the last character from buffer
 */
char Scanner::buffer_peek() {
  assert(!buffer.empty());
  return buffer[buffer.size() - 1];
}

Token Scanner::scan_string_literal() {
  do {
    consume();
    if (buffer_peek() == '\n' || buffer_peek() == EOF) {
      throw ScannerException(last_line, last_column, "String exceeds line");
    }
  } while (buffer_peek() != '\'');
  return prepare_token(TokenType::String, buffer, buffer);
}

bool Scanner::is_digit(char c, int numeral_system) {
  c = (char)std::tolower(c);
  if (numeral_system == 10)
    return '0' <= c && c <= '9';
  if (numeral_system == 16)
    return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
  if (numeral_system == 8)
    return '0' <= c && c <= '7';

  assert(numeral_system == 2);
  return '0' <= c && c <= '1';
}

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
    switch (current_state) {
    case number:
      c = consume();
      if (is_digit(c, 10)) {
        // consume
      } else if (c == '.' && try_consume([](char c) {
                   return is_digit(c, 10);
                 })) { // check next
        current_state = state::number_after_dot;
        type = TokenType::Real;
      } else if (c == 'e' &&
                 try_consume([](char c) { return is_digit(c, 10); })) {
        current_state = state::number_after_e;
        type = TokenType::Real;
      } else {
        unconsume(); // give back . or e
        current_state = finish;
      }
      break;
    case number_after_dot:
      if (try_consume([](char c) { return is_digit(c, 10); })) {
        current_state = number_after_dot;
      } else {
        c = consume();
        if (c == 'e' && try_consume([](char c) { return is_digit(c, 10); })) {
          current_state = number_after_e;
        } else {
          unconsume();
          current_state = finish;
        }
      }
      break;
    case number_after_e:
      if (try_consume([](char c) { return is_digit(c, 10); })) {
        current_state = number_after_e;
      } else {
        current_state = finish;
      }
      break;
    case hex_number:
      if (try_consume([](char c) { return is_digit(c, 16); })) {
        current_state = hex_number;
      } else {
        current_state = finish;
      }
      break;
    case octa_number:
      if (try_consume([](char c) { return is_digit(c, 8); })) {
        current_state = octa_number;
      } else {
        current_state = finish;
      }
      break;
    case bin_number:
      if (try_consume([](char c) { return is_digit(c, 2); })) {
        current_state = bin_number;
      } else {
        current_state = finish;
      }
      break;
    }
  }
  if (type == Integer) {
    return prepare_token(
        type, lexer::Scanner::get_integer_value(buffer, numeral_system),
        buffer);
  }
  return prepare_token(type, lexer::Scanner::get_real_value(buffer), buffer);
}

/*
 * Skip block comment line { }
 */
void Scanner::skip_block_comment() {
  for (;;) {
    consume();
    if (buffer_peek() == '}') {
      return;
    } else if (buffer_peek() == EOF) {
      throw ScannerException(last_line, last_column,
                             "Unterminated block comment");
    }
  }
}

/*
 * Skip block comment line (* *)
 */
void Scanner::skip_block_comment_1() {
  for (;;) {
    consume();
    if (buffer_peek() == '*' && try_consume(')')) {
      return;
    } else if (buffer_peek() == EOF) {
      throw ScannerException(last_line, last_column,
                             "Unterminated block comment");
    }
  }
}

void Scanner::skip_line_comment() {
  for (;;) {
    consume();
    if (buffer_peek() == '\n' || buffer_peek() == EOF) {
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
  while (is_remainig_of_identifier(peek())) {
    consume();
  }
  auto type = TokenType::Id;

  std::string buffer_in_lower = buffer;
  std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                 buffer_in_lower.begin(), ::tolower);

  if (std::find(keywords.begin(), keywords.end(), buffer_in_lower) !=
      keywords.end()) {
    type = TokenType::Keyword;
  }

  return prepare_token(type, buffer_in_lower, buffer);
}

bool Scanner::eof() const { return is_eof; }

std::string Scanner::get_integer_value(std::string raw,
                                       int numeral_system) const {
  long long int result = 0;

  for (size_t i = (numeral_system != 10); i < raw.size(); ++i) {
    result *= numeral_system;
    char c = (char)tolower(raw[i]);
    if ('0' <= c && c <= '9')
      result += c - '0';
    if ('a' <= c && c <= 'z')
      result += c - 'a' + 10;
    if (INTEGER_MAX < result) {
      throw ScannerException(last_line, last_column, "Integer overflow");
    }
  }
  return std::to_string((int)result);
}

std::string Scanner::get_real_value(const std::string &raw) {
  std::stringstream ss;
  ss.setf(std::ios::scientific);
  ss.precision(12);
  try {
    ss << std::stof(raw);
    return ss.str();
  } catch (...) {
    ss << INFINITY;
    return ss.str();
  }
}

Token Scanner::prepare_token(TokenType type, const std::string &value,
                             const std::string &raw_value) const {
  return {last_line, last_column, type, value, raw_value};
}
} // namespace lexer
