//
// Created by andrey on 28.10.2022.
//

#include "buffered_istream.h"

#include <cassert>

namespace scanner {
char BufferedIStream::consume() {
    char c = (char)input_stream.get();
    buffer.push_back(c);
    if (c == EOF) {
        is_eof = true;
    }
    if (c == '\n') {
        column_after_new_line = current_line;
        ++current_line;
        current_column = 1;
    } else {
        ++current_column;
    }
    return c;
}

char BufferedIStream::try_consume(char c) {
    if (peek() == c) {
        consume();
        return true;
    }
    return false;
}

char BufferedIStream::unconsume() {
    input_stream.unget();
    if (!buffer.empty()) {
        if (buffer_peek() != '\n') {
            --current_column;
        } else {
            current_line--;
            current_column = column_after_new_line;
        }
        buffer.pop_back();
        return buffer.back();
    } else {
        assert(true);
    }
    return EOF;
}

char BufferedIStream::try_consume(const std::function<bool(char)> &func) {
    if (func(peek())) {
        consume();
        return true;
    }
    return false;
}

char BufferedIStream::peek() { return (char)input_stream.peek(); }

[[maybe_unused]] std::string BufferedIStream::get_buffer() { return buffer; }

void BufferedIStream::add_to_buffer(char c) { buffer.push_back(c); }

[[maybe_unused]] void BufferedIStream::clear_buffer() { buffer.clear(); }

char BufferedIStream::buffer_peek() {
    assert(!buffer.empty());
    return buffer[buffer.size() - 1];
}

unsigned int BufferedIStream::get_current_line() const { return current_line; }

unsigned int BufferedIStream::get_current_column() const {
    return current_column;
}

[[nodiscard]] bool BufferedIStream::eof() const { return is_eof; }

}  // namespace scanner