#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

#include <utility>

#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "node.h"

using namespace scanner;

namespace simpleparser {
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

    SyntaxNodePointer parse_expression();
    SyntaxNodePointer parse_term();
    SyntaxNodePointer parse_factor();
};

class SyntaxException : public TinyPascalException {
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
