#ifndef TINY_TINY_PASCAL_EXCEPTION_H
#define TINY_TINY_PASCAL_EXCEPTION_H

#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <utility>

#include "position.h"

class TinyPascalException : virtual public std::exception {
   public:
    explicit TinyPascalException(std::string message)
        : message(std::move(message)) {}
    explicit TinyPascalException(Position pos, std::string &&message) {
        std::ostringstream os;
        os << "(" << pos.first << ", " << pos.second << "): " << message;
        this->message = os.str();
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

   protected:
    std::string message;
};

class ScannerException : virtual public TinyPascalException {
   public:
    explicit ScannerException(std::string message) noexcept
        : TinyPascalException(std::move(message)) {}
    explicit ScannerException(Position pos, std::string &&message) noexcept
        : TinyPascalException(pos, std::forward<std::string &&>(message)) {}
};

class ParserException : virtual public TinyPascalException {
   public:
    explicit ParserException(std::string message) noexcept
        : TinyPascalException(std::move(message)) {}
    explicit ParserException(Position pos, std::string &&message) noexcept
        : TinyPascalException(pos, std::forward<std::string &&>(message)) {}
};

class SemanticException : virtual public TinyPascalException {
   public:
    explicit SemanticException(std::string message) noexcept
        : TinyPascalException(std::move(message)) {}
    explicit SemanticException(Position pos, std::string &&message) noexcept
        : TinyPascalException(pos, std::forward<std::string &&>(message)) {}
};

template <typename Derived>
class ExceptionBuilder {
   public:
    ExceptionBuilder() = default;
    ~ExceptionBuilder() = default;
    explicit ExceptionBuilder(Position pos)
        : pos(std::make_shared<Position>(pos.first, pos.second)) {}
    template <typename T>
    ExceptionBuilder &operator<<(T &&x) {
        os << x;
        return *this;
    }
    Derived operator<<([[maybe_unused]] void (*x)()) {
        if (pos != nullptr) {
            return Derived(*pos, os.str());
        }
        return Derived(os.str());
    }

   protected:
    std::ostringstream os;
    std::shared_ptr<Position> pos = nullptr;
};

extern void make_exc_end();

template <typename Derived>
ExceptionBuilder<Derived> make_exc() {
    return ExceptionBuilder<Derived>();
}

template <typename Derived>
ExceptionBuilder<Derived> make_exc(Position &&pos) {
    return ExceptionBuilder<Derived>(pos);
}

#endif  // TINY_TINY_PASCAL_EXCEPTION_H
