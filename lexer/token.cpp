#include <iomanip>
#include <sstream>

#include "token.h"

namespace lexer
{
std::ostream &operator<<(std::ostream &os, const lexer::Token &token)
{
    os << token.line << "\t" << token.column << "\t" << token.type << "\t";
    switch (token.type)
    {
    case LITERAL_INTEGER:
        os << std::get<Integer>(token.value);
        break;
    case LITEREAL_DOUBLE:
        os << std::get<Double>(token.value);
        break;
    case LITERAL_STRING:
        os << std::get<String>(token.value);
        break;
    case ID:
        os << std::get<String>(token.value);
        break;
    case KEYWORD:
        os << std::get<Keywords>(token.value);
        break;
    case COMMENT:
        break;
    case OPER:
        os << std::get<Operators>(token.value);
        break;
    case SEPERATOR:
        os << std::get<Separators>(token.value);
        break;
    case eof:
        os << "EOF";
    case INVALID:
        break;
    }
    os << "\t" << token.raw_value;
    return os;
}

std::string Token::to_string() const
{
    std::ostringstream ss;
    ss << (*this);
    return ss.str();
}

TokenType Token::get_type()
{
    return type;
}
} // namespace lexer
