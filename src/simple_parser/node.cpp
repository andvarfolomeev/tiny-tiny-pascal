#include "node.h"

namespace simpleparser {
void BinOpNode::draw(std::ostream& os, int depth) {
    auto padding = 3;
    os << to_string() << std::endl;
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

double BinOpNode::calc() {
    if (token.get_value<Operators>() == Operators::SUB) {
        return left->calc() - right->calc();
    } else if (token.get_value<Operators>() == Operators::ADD) {
        return left->calc() + right->calc();
    } else if (token.get_value<Operators>() == Operators::MUL) {
        return left->calc() * right->calc();
    }
    return left->calc() / right->calc();
}

void NumberNode::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << to_string() << std::endl;
}

std::string NumberNode::to_string() {
    if (token == TokenType::LITERAL_DOUBLE) {
        return std::to_string(token.get_value<Double>());
    }
    return std::to_string(token.get_value<Integer>());
}

double NumberNode::calc() {
    if (token == TokenType::LITERAL_INTEGER) {
        return token.get_value<Integer>();
    }
    return token.get_value<Double>();
}

void IdNode::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << to_string() << std::endl;
}

std::string IdNode::to_string() { return token.get_raw_value(); }

double IdNode::calc() { return 0; }

double SyntaxNode::calc() { return 0; }
}  // namespace simpleparser