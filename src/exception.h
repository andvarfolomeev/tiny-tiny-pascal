#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class TinyPascalException : std::exception {
   public:
    [[nodiscard]] const char *what() const noexcept override = 0;
};

#endif  // EXCEPTION_H
