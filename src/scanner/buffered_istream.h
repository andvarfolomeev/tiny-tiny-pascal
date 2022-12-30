#ifndef LEXER_BUFFERED_ISTREAM_H
#define LEXER_BUFFERED_ISTREAM_H

#include <fstream>
#include <functional>
#include <sstream>

#include "../position.h"

namespace scanner {
class BufferedIStream {
   public:
    explicit BufferedIStream(std::ifstream &input_stream)
        : input_stream(input_stream),
          pos({1, 1}),
          column_after_new_line(1),
          is_eof(false) {}

    [[nodiscard]] bool eof() const;

    Position get_pos() const;
    [[nodiscard]] int get_current_line() const;
    [[nodiscard]] int get_current_column() const;

   private:
    std::ifstream &input_stream;
    Position pos;
    int column_after_new_line;
    std::string buffer;
    bool is_eof;

   protected:
    /**
     * get next char and save to buffer
     * @return
     */
    char consume();

    /**
     * if next char equal c then call consume
     * @param c
     * @return
     */
    char try_consume(char c);

    /**
     * if result of function call is true then call consume
     * @param c
     * @return
     */
    char try_consume(const std::function<bool(char)> &func);

    char unconsume();

    /**
     * get next char without move to them
     * @return
     */
    char peek();

    /**
     *
     * @return buffer field
     */
    std::string get_buffer();

    /**
     * add char to buffer
     * @param c
     */
    void add_to_buffer(char c);

    /**
     * call clear of string buffer
     */
    void clear_buffer();

    /**
     * get last char in buffer
     * @return
     */
    char buffer_peek();
};

}  // namespace scanner
#endif  // LEXER_BUFFERED_ISTREAM_H
