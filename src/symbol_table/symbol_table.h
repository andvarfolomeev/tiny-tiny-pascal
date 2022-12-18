#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "symbol.h"

class SymbolTable {
   public:
    SymbolTable() {}
    std::shared_ptr<Symbol> at(std::string name);
    void push(std::string name, std::shared_ptr<Symbol> symbol);

   protected:
    std::map<std::string, std::shared_ptr<Symbol>> data;
};

#endif
