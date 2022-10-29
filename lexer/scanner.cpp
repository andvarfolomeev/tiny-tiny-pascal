#include "scanner.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <magic_enum.hpp>

#include "../types.h"
#include "token.h"
#include "token_type.h"
#include "token_value_types.h"

namespace lexer {

Token Scanner::next_token() {
    char c;

    do {
        c = consume();

        // save position before call skip_*_comment
        // for correct position in exception
        last_column = get_current_column() - 1;
        last_line = get_current_line();

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
    last_column = get_current_column() - 1;
    last_line = get_current_line();

    clear_buffer();
    add_to_buffer(c);

    switch (c) {
        case EOF:
            return prepare_token(TokenType::eof, "EOF", "EOF");
        case '+':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::ADDASSIGN,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::ADD, get_buffer());
        case '-':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::SUBASSIGN,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::SUB, get_buffer());
        case '*':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::MULASSIGN,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::MUL, get_buffer());
        case '/':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::QUOASSIGN,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::QUO, get_buffer());
        case '<':
            if (try_consume('>')) {
                return prepare_token(TokenType::OPER, Operators::NEQ,
                                     get_buffer());
            } else if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::LEQ,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::LES, get_buffer());
        case ':':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::ASSIGN,
                                     get_buffer());
            }
            return prepare_token(TokenType::SEPERATOR, Separators::COLON,
                                 get_buffer());
        case '>':
            if (try_consume('=')) {
                return prepare_token(TokenType::OPER, Operators::GEQ,
                                     get_buffer());
            }
            return prepare_token(TokenType::OPER, Operators::GTR, get_buffer());
        case '=':
            return prepare_token(TokenType::OPER, Operators::EQL, get_buffer());
        case '^':
            return prepare_token(TokenType::OPER, Operators::DEREF,
                                 get_buffer());
        case '(':
            return prepare_token(TokenType::SEPERATOR, Separators::LPAREN,
                                 get_buffer());
        case ')':
            return prepare_token(TokenType::SEPERATOR, Separators::RPAREN,
                                 get_buffer());
        case '[':
            return prepare_token(TokenType::SEPERATOR, Separators::LBRACK,
                                 get_buffer());
        case ']':
            return prepare_token(TokenType::SEPERATOR, Separators::RBRACK,
                                 get_buffer());
        case ',':
            return prepare_token(TokenType::SEPERATOR, Separators::COMMA,
                                 get_buffer());
        case ';':
            return prepare_token(TokenType::SEPERATOR, Separators::SEMICOLON,
                                 get_buffer());
        case '.':
            if (try_consume('.')) {
                return prepare_token(TokenType::SEPERATOR, Separators::ELLIPSIS,
                                     get_buffer());
            }
            return prepare_token(TokenType::SEPERATOR, Separators::PERIOD,
                                 get_buffer());
        case '\'':
            return scan_string_literal(false);
        case '$':
            if (try_consume([](char c) { return is_digit(c, 16); })) {
                scan_number_literal(16);
                return prepare_token(TokenType::LITERAL_INTEGER,
                                     get_integer_value(get_buffer(), 16),
                                     get_buffer());
            }
            break;
        case '&':
            if (try_consume([](char c) { return is_digit(c, 8); })) {
                scan_number_literal(8);
                return prepare_token(TokenType::LITERAL_INTEGER,
                                     get_integer_value(get_buffer(), 8),
                                     get_buffer());
            }
            break;
        case '%':
            if (try_consume([](char c) { return is_digit(c, 2); })) {
                scan_number_literal(2);
                return prepare_token(TokenType::LITERAL_INTEGER,
                                     get_integer_value(get_buffer(), 2),
                                     get_buffer());
            }
            break;
        case '#':
            return scan_string_literal(true);
        default:
            if (is_digit(buffer_peek())) {
                auto type = scan_number_literal(10);

                assert(type == TokenType::LITERAL_INTEGER ||
                       type == TokenType::LITEREAL_DOUBLE);

                if (type == TokenType::LITERAL_INTEGER) {
                    return prepare_token(type,
                                         get_integer_value(get_buffer(), 10),
                                         get_buffer());
                } else {
                    return prepare_token(type, get_double_value(get_buffer()),
                                         get_buffer());
                }
            }
            if (is_start_of_identifier(buffer_peek())) {
                scan_identifier();

                // check keyword
                std::string buffer_in_upper = get_buffer();
                std::transform(buffer_in_upper.begin(), buffer_in_upper.end(),
                               buffer_in_upper.begin(), ::toupper);

                auto value_in_keyword_enum =
                    magic_enum::enum_cast<Keywords>(buffer_in_upper);

                if (value_in_keyword_enum.has_value()) {
                    return prepare_token(TokenType::KEYWORD,
                                         value_in_keyword_enum.value(),
                                         get_buffer());
                }

                return prepare_token(TokenType::ID, get_buffer(), get_buffer());
            }
            break;
    }

    // means that the token type is invalid
    throw UnexpectedTokenException(last_line, last_column);
}

bool Scanner::is_space(char c) { return c == '\t' || c == ' ' || c == '\n'; }

Token Scanner::scan_string_literal(bool start_with_hash) {
    enum state {
        begin,
        string,
        pre_spec_char,
        pre_read_number,
        read_number,
        post_read_number,
        finish
    };

    state current_state = begin;
    std::string value_buffer;

    int number_of_special_symbol = 0;

    while (current_state != finish) {
        switch (current_state) {
            case begin:
                if (start_with_hash) {
                    current_state = pre_read_number;
                } else {
                    current_state = string;
                }
                break;
            case string:
                if (try_consume('\'')) {
                    current_state = pre_spec_char;
                } else if (buffer_peek() == '\n' || buffer_peek() == EOF) {
                    throw StringExceedsLineException(last_line, last_column);
                } else {
                    value_buffer.push_back(peek());
                    consume();
                }
                break;
            case pre_spec_char:
                if (try_consume('#')) {
                    current_state = pre_read_number;
                } else {
                    current_state = finish;
                }
                break;
            case pre_read_number:
                number_of_special_symbol = 0;
                if (try_consume([](char c) { return is_digit(c, 10); })) {
                    number_of_special_symbol =
                        number_of_special_symbol * 10 + buffer_peek() - '0';
                    current_state = read_number;
                } else {
                    throw IllegalCharacterException(
                        get_current_line(), get_current_column(), peek());
                }
                break;
            case read_number:
                if (try_consume([](char c) { return is_digit(c, 10); })) {
                    number_of_special_symbol =
                        number_of_special_symbol * 10 + buffer_peek() - '0';
                    current_state = read_number;
                } else {
                    current_state = post_read_number;
                }
                break;
            case post_read_number:
                value_buffer.push_back((char)number_of_special_symbol);
                if (try_consume('#')) {
                    current_state = pre_read_number;
                } else if (try_consume('\'')) {
                    current_state = string;
                } else {
                    current_state = finish;
                }
                break;
            case finish:
                break;
        }
    }

    return prepare_token(TokenType::LITERAL_STRING, value_buffer, get_buffer());
}

bool Scanner::is_digit(char c, int numeral_system) {
    c = (char)std::tolower(c);
    if (numeral_system == 10) {
        return '0' <= c && c <= '9';
    }
    if (numeral_system == 16) {
        return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f');
    }
    if (numeral_system == 8) {
        return '0' <= c && c <= '7';
    }

    assert(numeral_system == 2);
    return '0' <= c && c <= '1';
}

int Scanner::digits(int numeral_system) {
    int count_of_digits = 0;
    while (try_consume([=](char c) { return is_digit(c, numeral_system); })) {
        ++count_of_digits;
    }
    return count_of_digits;
}

TokenType Scanner::scan_number_literal(int numeral_system) {
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

    auto type = TokenType::LITERAL_INTEGER;

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
                           })) {  // check next
                    current_state = state::number_after_dot;
                    type = TokenType::LITEREAL_DOUBLE;
                } else if (c == 'e' && try_consume([](char c) {
                               return is_digit(c, 10);
                           })) {
                    current_state = state::number_after_e;
                    type = TokenType::LITEREAL_DOUBLE;
                } else {
                    unconsume();  // give back . or e
                    current_state = finish;
                }
                break;
            case number_after_dot:
                if (try_consume([](char c) { return is_digit(c, 10); })) {
                    current_state = number_after_dot;
                } else {
                    c = consume();
                    if (c == 'e' &&
                        try_consume([](char c) { return is_digit(c, 10); })) {
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
            case finish:
                break;
        }
    }

    return type;
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
            throw UnterminatedBlockCommentException(last_line, last_column);
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
            throw UnterminatedBlockCommentException(last_line, last_column);
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

void Scanner::scan_identifier() {
    while (is_remainig_of_identifier(peek())) {
        consume();
    }
}

Integer Scanner::get_integer_value(std::string raw, int numeral_system) const {
    long long int result = 0;

    for (size_t i = (numeral_system != 10); i < raw.size(); ++i) {
        result *= numeral_system;
        char c = (char)tolower(raw[i]);
        if ('0' <= c && c <= '9') {
            result += c - '0';
        }
        if ('a' <= c && c <= 'z') {
            result += c - 'a' + 10;
        }
        if (INTEGER_MAX < result) {
            throw IntegerOverflowException(last_line, last_column);
        }
    }
    return (Integer)result;
}

Double Scanner::get_double_value(const std::string& raw) {
    std::stringstream ss;
    ss.setf(std::ios::scientific);
    ss.precision(12);
    try {
        return std::stof(raw);
    } catch (...) {
        return INFINITY;
    }
}

Token Scanner::prepare_token(TokenType type, const TokenValue& value,
                             const std::string& raw_value) const {
    return {last_line, last_column, type, value, raw_value};
}

}  // namespace lexer
