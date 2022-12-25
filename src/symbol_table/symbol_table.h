#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "symbol.h"

class SymbolTable {
   public:
    SymbolTable() {}
    std::shared_ptr<Symbol> get(std::string name);
    void del(std::string name);
    void push(std::string name, std::shared_ptr<Symbol> symbol);
    static std::shared_ptr<SymbolTable> get_with_builtin();
    void draw(std::ostream &os, int depth = 0);
    int size();
    std::map<std::string, std::shared_ptr<Symbol>>::iterator begin();
    std::map<std::string, std::shared_ptr<Symbol>>::iterator end();

   protected:
    std::map<std::string, std::shared_ptr<Symbol>> data;
    std::vector<std::string> ordered_names;
};

#endif
