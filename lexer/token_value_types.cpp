#include "token_value_types.h"

#include <magic_enum.hpp>

namespace lexer {
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
}  // namespace lexer
