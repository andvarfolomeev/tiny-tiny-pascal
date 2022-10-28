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
  char consume();
  char try_consume(char c);
  char try_consume(const std::function<bool(char)> &func);
  char unconsume();

  char peek();

  [[maybe_unused]] std::string get_buffer();
  void add_to_buffer(char c);
  [[maybe_unused]] void clear_buffer();
  char buffer_peek();

  [[nodiscard]] unsigned int get_current_line() const;
  [[nodiscard]] unsigned int get_current_column() const;

public:
  explicit BufferedIStream(std::ifstream &input_stream)
      : input_stream(input_stream), current_line(1), current_column(1),
        column_after_new_line(1), is_eof(false) {}

  [[nodiscard]] bool eof() const;
};

} // namespace lexer
#endif // LEXER_BUFFERED_ISTREAM_H
