#include "token.h"

#include <iomanip>
#include <sstream>

namespace scanner {
std::ostream &operator<<(std::ostream &os, const scanner::Token &token) {
    os << token.line << "\t" << token.column << "\t" << token.type << "\t";
    switch (token.type) {
        case LITERAL_INTEGER:
            os << token.get_value<Integer>();
            break;
        case LITERAL_DOUBLE:
            os << token.get_value<Double>();
            break;
        case ID:
        case LITERAL_STRING:
            os << token.get_value<String>();
            break;
        case KEYWORD:
            os << token.get_value<Keywords>();
            break;
        case COMMENT:
            break;
        case OPER:
            os << token.get_value<Operators>();
            break;
        case SEPERATOR:
            os << token.get_value<Separators>();
            break;
        case eof:
            os << "EOF";
        case INVALID:
            break;
    }
    os << "\t" << token.raw_value;
    return os;
}

std::string Token::to_string() const {
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

TokenType Token::get_type() { return type; }

template <typename T>
T Token::get_value() const {
    return std::get<T>(value);
}

}  // namespace scanner
