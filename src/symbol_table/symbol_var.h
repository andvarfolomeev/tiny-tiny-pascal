#ifndef SYMBOL_VAR_H
#define SYMBOL_VAR_H

#include "symbol.h"
#include "symbol_type.h"

class SymbolVar : public Symbol {
   public:
    SymbolVar(std::string name, std::shared_ptr<SymbolType> type)
        : Symbol(name), type(std::move(type)) {}

   protected:
    std::shared_ptr<SymbolType> type;
};

class SymbolVarParam : public SymbolVar {
   public:
    SymbolVarParam(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
};

#endif