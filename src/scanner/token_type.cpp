#include "token_type.h"

#include "magic_enum.hpp"

namespace scanner {
std::ostream &operator<<(std::ostream &os, const TokenType &type) {
    os << magic_enum::enum_name(type);
    return os;
}
}  // namespace scanner