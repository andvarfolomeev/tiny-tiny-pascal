#include "simple_parser.h"

#include "magic_enum.hpp"

namespace simpleparser {
SimpleParser::SimpleParser(Scanner scanner)
    : scanner(scanner), current_token(scanner.next_token()) {}

Double Node::calc() {
    if (token.get_type() == TokenType::OPER &&
        token.get_value<Operators>() == Operators::ADD) {
        return left->calc() + right->calc();
    } else if (token.get_type() == TokenType::OPER &&
               token.get_value<Operators>() == Operators::SUB) {
        return left->calc() - right->calc();
    } else if (token.get_type() == TokenType::OPER &&
               token.get_value<Operators>() == Operators::MUL) {
        return left->calc() * right->calc();
    } else if (token.get_type() == TokenType::OPER &&
               token.get_value<Operators>() == Operators::QUO) {
        return left->calc() / right->calc();
    } else if (token.get_type() == TokenType::LITERAL_DOUBLE) {
        return token.get_value<Double>();
    } else if (token.get_type() == TokenType::LITERAL_INTEGER) {
        return token.get_value<Integer>();
    }
    return 0;
}

void draw_path(std::ostream &os, int depth, bool is_right, int right_count,
               bool end_of_branch) {
    auto padding = 3;
    for (int i = 0; i < depth * padding; ++i) {
        os << " ";
    }
    if (is_right)
        os << "└──";
    else
        os << "└──";
}

void Node::_draw_tree(std::ostream &os, int depth, int right_count) {
    os << to_string() << std::endl;
    if (left == nullptr || right == nullptr) return;

    auto right_ended = right->right == nullptr || right->left == nullptr;
    draw_path(os, depth + 1, true, right_count, right_ended);
    right->_draw_tree(os, depth + 1, right_count + 1);

    auto left_ended = left->right == nullptr || left->left == nullptr;
    draw_path(os, depth + 1, false, right_count, left_ended);
    left->_draw_tree(os, depth + 1, right_count);
}

std::string Node::to_string() {
    if (token.get_type() == TokenType::OPER) {
        return "(" + token.get_raw_value() + ")";
    }
    if (token.get_type() == TokenType::LITERAL_DOUBLE) {
        return std::to_string(token.get_value<Double>());
    }
    return std::to_string(token.get_value<Integer>());
}

void Node::draw_tree(std::ostream &os) { _draw_tree(os, 0, 0); }

Node *SimpleParser::parse_expression() {
    auto left = parse_term();
    auto token = current_token;
    while (token.get_type() == TokenType::OPER &&
           (token.get_value<Operators>() == Operators::ADD ||
            token.get_value<Operators>() == Operators::SUB)) {
        current_token = scanner.next_token();
        left = new Node(token, left, parse_term());
        token = current_token;
    }
    return left;
}
Node *SimpleParser::parse_term() {
    auto left = parse_factor();
    auto token = current_token;
    while (token.get_type() == TokenType::OPER &&
           (token.get_value<Operators>() == Operators::MUL ||
            token.get_value<Operators>() == Operators::QUO)) {
        current_token = scanner.next_token();
        left = new Node(token, left, parse_term());
        token = current_token;
    }
    return left;
}
Node *SimpleParser::parse_factor() {
    auto token = current_token;
    if (token.get_type() == TokenType::LITERAL_INTEGER ||
        token.get_type() == TokenType::LITERAL_DOUBLE ||
        token.get_type() == TokenType::ID) {
        current_token = scanner.next_token();
        return new Node(token, nullptr, nullptr);
    }
    if (token.get_type() == TokenType::SEPERATOR &&
        token.get_value<Separators>() == Separators::LPAREN) {
        current_token = scanner.next_token();
        auto expression = parse_expression();

        if (current_token.get_type() != TokenType::SEPERATOR ||
            !(current_token.get_type() == TokenType::SEPERATOR &&
              current_token.get_value<Separators>() == Separators::RPAREN)) {
            throw SyntaxException(scanner.get_current_line(),
                                  scanner.get_current_column(), "Expected )");
        }
        current_token = scanner.next_token();
        return expression;
    }
    throw SyntaxException(scanner.get_current_line(),
                          scanner.get_current_column(), "Factor expected");
}
}  // namespace simpleparser