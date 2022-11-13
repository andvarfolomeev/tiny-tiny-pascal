#include "token.h"

#include <iomanip>
#include <sstream>

#include "magic_enum.hpp"

namespace scanner {
std::ostream &operator<<(std::ostream &os, const Operators &op) {
    os << magic_enum::enum_name(op);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Separators &sep) {
    os << magic_enum::enum_name(sep);
    return os;
}

std::ostream &operator<<(std::ostream &os, const Keywords &keyword) {
    os << magic_enum::enum_name(keyword);
    return os;
}

std::ostream &operator<<(std::ostream &os, const TokenType &type) {
    os << magic_enum::enum_name(type);
    return os;
}

std::ostream &operator<<(std::ostream &os, const scanner::Token &token) {
    os << std::setw(10) << std::left << token.line << std::setw(10) << std::left
       << token.column << std::setw(20) << std::left << token.type << "\t";
    switch (token.type) {
        case LITERAL_INTEGER:
            os << std::setw(30) << std::left << token.get_value<Integer>();
            break;
        case LITERAL_DOUBLE:
            os << std::setw(30) << std::left << token.get_value<Double>();
            break;
        case ID:
        case LITERAL_STRING:
            os << std::setw(30) << std::left << token.get_value<String>();
            break;
        case KEYWORD:
            os << std::setw(30) << std::left << token.get_value<Keywords>();
            break;
        case COMMENT:
            break;
        case OPER:
            os << std::setw(30) << std::left << token.get_value<Operators>();
            break;
        case SEPERATOR:
            os << std::setw(30) << std::left << token.get_value<Separators>();
            break;
        case eof:
            os << std::setw(30) << std::left << "EOF";
        case INVALID:
            break;
    }
    os << std::setw(30) << std::left << token.raw_value;
    return os;
}

bool operator==(const Token &token, const TokenType &type) {
    return token.type == type;
}

bool Token::is(std::vector<TokenType> types) {
    return std::find(types.begin(), types.end(), type) != types.end();
}

bool Token::is(std::vector<Operators> values) {
    if (type != TokenType::OPER) return false;
    return std::find(values.begin(), values.end(), get_value<Operators>()) !=
           values.end();
}

bool Token::is(std::vector<Separators> values) {
    if (type != TokenType::SEPERATOR) return false;
    return std::find(values.begin(), values.end(), get_value<Separators>()) !=
           values.end();
}

std::string Token::to_string() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

TokenType Token::get_type() { return type; }

std::string Token::get_raw_value() { return raw_value; }

template <typename T>
T Token::get_value() const {
    return std::get<T>(value);
}

bool operator==(const Token &token, const Operators &op) {
    if (token.type != TokenType::OPER) return false;
    return token.get_value<Operators>() == op;
}

bool operator==(const Token &token, const Separators &sep) {
    if (token.type != TokenType::SEPERATOR) return false;
    return token.get_value<Separators>() == sep;
}

}  // namespace scanner
