#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <memory>
#include <string>
#include <vector>

class Symbol {
   public:
    Symbol(std::string name) : name(std::move(name)) {}
    std::string get_name();

   protected:
    std::string name;
};

#endif  // SYMBOL_H
