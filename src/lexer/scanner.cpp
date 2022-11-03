#include "scanner.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include "../pascal_types.h"
#include "magic_enum.hpp"
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
            return scan_number_literal(16);
        case '&':
            return scan_number_literal(8);
        case '%':
            return scan_number_literal(2);
        case '#':
            return scan_string_literal(true);
        default:
            if (is_digit(buffer_peek())) {
                return scan_number_literal(10);
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

Token Scanner::scan_number_literal(int numeral_system) {
    assert(numeral_system == 10 || numeral_system == 16 ||
           numeral_system == 8 || numeral_system == 2);
    enum state {
        begin,

        dec,
        scale,
        pre_exp,
        exp_dec,
        exp_sign,

        pre_not_dec,
        not_dec,

        finish
    };

    std::string integer_part;
    std::string fraction_part;
    std::string exponent_part;

    TokenType type = TokenType::LITERAL_INTEGER;

    state current_state = begin;
    while (current_state != finish) {
        switch (current_state) {
            case begin:
                if (numeral_system == 10) {
                    integer_part.push_back(buffer_peek());
                    current_state = dec;
                } else {
                    current_state = pre_not_dec;
                }
                break;
            case dec:
                if (try_consume([](char c) { return is_digit(c); })) {
                    current_state = dec;
                    integer_part.push_back(buffer_peek());
                } else if (try_consume('.')) {
                    type = LITERAL_DOUBLE;
                    fraction_part.push_back(buffer_peek());
                    current_state = scale;
                } else if (try_consume('e') || try_consume('E')) {
                    type = LITERAL_DOUBLE;
                    exponent_part.push_back(buffer_peek());
                    current_state = pre_exp;
                } else {
                    current_state = finish;
                }
                break;
            case scale:
                type = TokenType::LITERAL_DOUBLE;
                if (try_consume([](char c) { return is_digit(c); })) {
                    fraction_part.push_back(buffer_peek());
                    current_state = scale;
                } else if (buffer_peek() == '.' && peek() == '.') {
                    unconsume();
                    current_state = finish;
                } else if (try_consume('e') || try_consume('E')) {
                    exponent_part.push_back(buffer_peek());
                    current_state = pre_exp;
                } else {
                    current_state = finish;
                }
                break;
            case pre_exp:
                type = TokenType::LITERAL_DOUBLE;
                if (try_consume([](char c) { return is_digit(c); })) {
                    exponent_part.push_back(buffer_peek());
                    current_state = exp_dec;
                } else if (try_consume('+') || try_consume('-')) {
                    exponent_part.push_back(buffer_peek());
                    current_state = exp_sign;
                } else {
                    throw IllegalCharacterException(
                        get_current_line(), get_current_column(), peek());
                }
                break;
            case exp_dec:
                if (try_consume([](char c) { return is_digit(c); })) {
                    current_state = exp_dec;
                    exponent_part.push_back(buffer_peek());
                } else {
                    current_state = finish;
                }
                break;
            case exp_sign:
                if (try_consume([](char c) { return is_digit(c); })) {
                    current_state = exp_dec;
                } else {
                    throw IllegalCharacterException(
                        get_current_line(), get_current_column(), peek());
                }
                break;
            case pre_not_dec:
                if (try_consume(
                        [=](char c) { return is_digit(c, numeral_system); })) {
                    current_state = not_dec;
                    integer_part.push_back(buffer_peek());
                } else {
                    throw InvalidIntegerExpressionException(
                        get_current_line(), get_current_column());
                }
                break;
            case not_dec:
                if (try_consume(
                        [=](char c) { return is_digit(c, numeral_system); })) {
                    integer_part.push_back(buffer_peek());
                    current_state = not_dec;
                } else if (try_consume('.')) {
                    type = LITERAL_DOUBLE;
                    current_state = finish;
                } else {
                    current_state = finish;
                }
                break;
            case finish:
                break;
        }
    }

    if (numeral_system == 10 && type == LITERAL_INTEGER) {
        return prepare_token(type,
                             get_integer_value(get_buffer(), numeral_system),
                             get_buffer());
    } else if (numeral_system == 10) {
        std::string value_buffer;
        value_buffer += integer_part;
        if (fraction_part.size() > 1) {
            value_buffer += fraction_part;
        }
        value_buffer += exponent_part;
        return prepare_token(type, get_double_value(value_buffer),
                             get_buffer());
    }

    // not dec
    if (type == LITERAL_INTEGER) {
        return prepare_token(type,
                             get_integer_value(get_buffer(), numeral_system),
                             get_buffer());
    }
    return prepare_token(
        type, (Double)get_integer_value(get_buffer(), numeral_system),
        get_buffer());
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
    try {
        return std::stod(raw);
    } catch (...) {
        return INFINITY;
    }
}

Token Scanner::prepare_token(TokenType type, const TokenValue& value,
                             const std::string& raw_value) const {
    return {last_line, last_column, type, value, raw_value};
}

}  // namespace lexer
