#include "node.h"

namespace simpleparser {
void BinOpNode::draw(std::ostream& os, int depth) {
    auto padding = 3;

    os << to_string() << std::endl;
    if (left == nullptr || right == nullptr) return;

    for (int i = 0; i < (depth + 1) * padding; ++i) {
        os << " ";
    }
    left->draw(os, depth + 1);

    for (int i = 0; i < (depth + 1) * padding; ++i) {
        os << " ";
    }
    right->draw(os, depth + 1);
}

std::string BinOpNode::to_string() { return token.get_raw_value(); }

void NumberNode::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << to_string() << std::endl;
}

std::string NumberNode::to_string() {
    if (token == TokenType::LITERAL_DOUBLE) {
        return std::to_string(token.get_value<Double>());
    }
    return std::to_string(token.get_value<Integer>());
}

void IdNode::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << to_string() << std::endl;
}

std::string IdNode::to_string() { return token.get_raw_value(); }
}  // namespace simpleparser