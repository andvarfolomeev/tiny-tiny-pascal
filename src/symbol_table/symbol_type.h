#ifndef SYMBOL_TYPE_H
#define SYMBOL_TYPE_H

#include "symbol.h"
#include "symbol_table.h"

class SymbolType : public Symbol {
   public:
    SymbolType(std::string name) : Symbol(name) {}
};

class SymbolInteger : public SymbolType {
   public:
    SymbolInteger() : SymbolType("integer") {}
};

class SymbolDouble : public SymbolType {
   public:
    SymbolDouble() : SymbolType("double") {}
};

class SymbolString : public SymbolType {
   public:
    SymbolString() : SymbolType("string") {}
};

class SymbolArray : public SymbolType {
   public:
    SymbolArray(std::shared_ptr<SymbolType> elem,
                std::vector<std::pair<size_t, size_t>> ranges)
        : SymbolType("array"), elem(elem), ranges(ranges) {}

   protected:
    std::shared_ptr<SymbolType> elem;
    std::vector<std::pair<size_t, size_t>> ranges;
};

class SymbolRecord : public SymbolType {
   public:
    SymbolRecord(std::shared_ptr<SymbolTable> fields)
        : SymbolType("record"), fields(std::move(fields)) {}

   protected:
    std::shared_ptr<SymbolTable> fields;
};

class SymbolTypeAlias : public SymbolType {
   public:
    SymbolTypeAlias(std::string name, std::shared_ptr<SymbolType> original)
        : SymbolType(name), original(std::move(original)) {}

   protected:
    std::shared_ptr<SymbolType> original;
};
#endif
