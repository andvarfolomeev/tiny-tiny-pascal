#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

#include <utility>

#include "../scanner/scanner.h"
#include "../scanner/token.h"

using namespace scanner;

namespace simpleparser {
class Node {
   protected:
    Token token;
    Node *left;
    Node *right;

    void _draw_tree(std::ostream &os, int depth, int right_count);

   public:
    explicit Node(Token token, Node *left, Node *right)
        : token(std::move(token)), left(left), right(right) {}

    Token get_token();

    Double calc();

    std::string to_string();

    void draw_tree(std::ostream &os);
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
