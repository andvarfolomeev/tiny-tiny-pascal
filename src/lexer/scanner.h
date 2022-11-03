#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

#include <any>
#include <fstream>
#include <sstream>
#include <utility>

#include "buffered_istream.h"
#include "token.h"
#include "token_value_types.h"

namespace lexer {
class Scanner : public BufferedIStream {
    unsigned int last_line, last_column;

    [[nodiscard]] Token prepare_token(TokenType type, const TokenValue &value,
                                      const std::string &raw_value) const;

    /**
     *
     * @param start_with_hash - if first char is # then true
     * @return
     */
    Token scan_string_literal(bool start_with_hash);

    /**
     *
     * @param numeral_system
     * @return
     */
    Token scan_number_literal(int numeral_system);

    void scan_identifier();

    static bool is_space(char c);
    static bool is_digit(char c, int numeral_system = 10);
    static bool is_start_of_identifier(char c);
    static bool is_remainig_of_identifier(char c);

    /**
     * skip { } comment
     */
    void skip_block_comment();

    /**
     * skip (* *) comment
     */
    void skip_block_comment_1();

    /**
     * skip // comment
     */
    void skip_line_comment();

    [[nodiscard]] Integer get_integer_value(std::string raw,
                                            int numeral_system) const;
    static Double get_double_value(const std::string &raw);

   public:
    explicit Scanner(std::ifstream &input_stream)
        : BufferedIStream(input_stream), last_line(1), last_column(1) {}

    Token next_token();
};

class ScannerException : public std::exception {
   public:
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

    [[maybe_unused]] explicit ScannerException(unsigned int current_line,
                                               unsigned int current_column,
                                               const std::string &message) {
        std::ostringstream string_stream;
        string_stream << "Line: " << current_line
                      << "; Column: " << current_column << "; " << message;
        this->message = string_stream.str();
    }

   protected:
    std::string message;
};

class UnexpectedTokenException : public ScannerException {
   public:
    UnexpectedTokenException(unsigned int current_line, unsigned current_column)
        : ScannerException(current_line, current_column, "Unexpected token") {}
};

class IllegalCharacterException : public ScannerException {
   public:
    IllegalCharacterException(unsigned int current_line,
                              unsigned current_column, char c)
        : ScannerException(current_line, current_column,
                           "Illegal character: ") {
        message.push_back(c);
    }
};

class StringExceedsLineException : public ScannerException {
   public:
    StringExceedsLineException(unsigned int current_line,
                               unsigned current_column)
        : ScannerException(current_line, current_column,
                           "String exceeds line") {}
};

class UnterminatedBlockCommentException : public ScannerException {
   public:
    UnterminatedBlockCommentException(unsigned int current_line,
                                      unsigned current_column)
        : ScannerException(current_line, current_column,
                           "Unterminated block comment") {}
};

class IntegerOverflowException : public ScannerException {
   public:
    IntegerOverflowException(unsigned int current_line, unsigned current_column)
        : ScannerException(current_line, current_column, "Integer overflow") {}
};

class InvalidIntegerExpressionException : public ScannerException {
   public:
    InvalidIntegerExpressionException(unsigned int current_line,
                                      unsigned current_column)
        : ScannerException(current_line, current_column,
                           "Invalid integer expression") {}
};
}  // namespace lexer

#endif  // LEXER_SCANNER_H
