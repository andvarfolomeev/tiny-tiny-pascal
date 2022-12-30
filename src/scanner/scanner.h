#ifndef LEXER_SCANNER_H
#define LEXER_SCANNER_H

#include <any>
#include <fstream>
#include <sstream>
#include <utility>

#include "../exception.h"
#include "../position.h"
#include "buffered_istream.h"
#include "token.h"

namespace scanner {
class Scanner : public BufferedIStream {
   public:
    explicit Scanner(std::ifstream &input_stream)
        : BufferedIStream(input_stream), first_pos({1, 1}) {}

    Token next_token();

   private:
    Position first_pos;

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

    static inline bool is_space(char c);
    static inline bool is_digit(char c, int numeral_system = 10);
    static inline bool is_start_of_identifier(char c);
    static inline bool is_remainig_of_identifier(char c);

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
};

}  // namespace scanner

#endif  // LEXER_SCANNER_H
