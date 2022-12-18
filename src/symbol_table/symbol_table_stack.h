#ifndef SYMBOL_TABLE_STACK_H
#define SYMBOL_TABLE_STACK_H

#include "symbol_table.h"

class SymbolTableStack {
   public:
    SymbolTableStack() {}
    void push(std::string name, std::shared_ptr<Symbol> value);
    std::shared_ptr<Symbol> at(std::string name);
    void alloc();
   private:
    std::vector<std::shared_ptr<SymbolTable>> data;
};

#endif
