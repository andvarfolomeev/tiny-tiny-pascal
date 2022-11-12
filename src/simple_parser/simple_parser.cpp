#include "simple_parser.h"

#include <memory>

namespace simpleparser {
SimpleParser::SimpleParser(Scanner scanner)
    : scanner(scanner), current_token(scanner.next_token()) {}

SyntaxNodePointer SimpleParser::parse_expression() {
    auto left = parse_term();
    auto token = current_token;
    while (token == TokenType::OPER &&
           token.check_value<Operators>({Operators::ADD, Operators::SUB})) {
        current_token = scanner.next_token();
        left = std::make_shared<BinOpNode>(token, left, parse_term());
        token = current_token;
    }
    return left;
}

SyntaxNodePointer SimpleParser::parse_term() {
    auto left = parse_factor();
    auto token = current_token;
    while (token == TokenType::OPER &&
           token.check_value<Operators>({Operators::MUL, Operators::QUO})) {
        current_token = scanner.next_token();
        left = std::make_shared<BinOpNode>(token, left, parse_term());
        token = current_token;
    }
    return left;
}

SyntaxNodePointer SimpleParser::parse_factor() {
    auto token = current_token;
    if (token == TokenType::SEPERATOR &&
        token.check_value<Separators>({Separators::RPAREN})) {
        throw SyntaxException(scanner.get_current_line(),
                              scanner.get_current_column(), "Unexpected )");
    }
    if (token.check_type(
            {TokenType::LITERAL_INTEGER, TokenType::LITERAL_DOUBLE})) {
        current_token = scanner.next_token();
        return std::make_shared<NumberNode>(token);
    }
    if (token == TokenType::ID) {
        current_token = scanner.next_token();
        return std::make_shared<IdNode>(token);
    }
    if (token == TokenType::SEPERATOR &&
        token.check_value<Separators>({Separators::LPAREN})) {
        current_token = scanner.next_token();
        auto expression = parse_expression();
        if (current_token != TokenType::SEPERATOR ||
            !(current_token == TokenType::SEPERATOR &&
              current_token.check_value<Separators>({Separators::RPAREN}))) {
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