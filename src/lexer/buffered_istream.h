#ifndef LEXER_BUFFERED_ISTREAM_H
#define LEXER_BUFFERED_ISTREAM_H

#include <fstream>
#include <functional>
#include <sstream>

namespace lexer {
class BufferedIStream {
    std::ifstream &input_stream;
    unsigned int current_line, current_column, column_after_new_line;
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

    [[nodiscard]] unsigned int get_current_line() const;
    [[nodiscard]] unsigned int get_current_column() const;

   public:
    explicit BufferedIStream(std::ifstream &input_stream)
        : input_stream(input_stream),
          current_line(1),
          current_column(1),
          column_after_new_line(1),
          is_eof(false) {}

    [[nodiscard]] bool eof() const;
};

}  // namespace lexer
#endif  // LEXER_BUFFERED_ISTREAM_H
