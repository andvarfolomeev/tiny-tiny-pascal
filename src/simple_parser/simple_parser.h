#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

#include <utility>

#include "../lexer/scanner.h"
#include "../lexer/token.h"

using namespace lexer;

namespace simpleparser {
class Node {
   protected:
    Token token;
    Node *left;
    Node *right;

   public:
    explicit Node(Token token, Node *left, Node *right)
        : token(std::move(token)), left(left), right(right) {}

    double calc();
};

/**
 * expression ::= term { '+' term | term '-' term }
 * term ::= factor { '*' factor | '/' factor }
 * factor ::= number | identifier | '(' expression ')'
 */
class SimpleParser {
    Scanner scanner;
    Token current_token;

   public:
    explicit SimpleParser(Scanner scanner);

    Node *parse_expression();
    Node *parse_term();
    Node *parse_factor();
};

class SyntaxException : public std::exception {
   protected:
    std::string message;

   public:
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

    [[maybe_unused]] explicit SyntaxException(unsigned int current_line,
                                              unsigned int current_column,
                                              const std::string &message) {
        std::ostringstream string_stream;
        string_stream << "Line: " << current_line
                      << "; Column: " << current_column << "; " << message;
        this->message = string_stream.str();
    }
};

}  // namespace simpleparser
#endif  // SIMPLE_PARSER_H
