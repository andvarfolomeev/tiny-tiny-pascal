#include "simple_parser.h"

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