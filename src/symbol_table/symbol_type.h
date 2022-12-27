#ifndef SYMBOL_TYPE_H
#define SYMBOL_TYPE_H

#include "symbol.h"
#include "symbol_table.h"

class SymbolType : public Symbol {
   public:
    explicit SymbolType(std::string name) : Symbol(name) {}
    ~SymbolType() override = default;
    bool is_type() final { return true; }
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
    virtual SymbolType* resolve_alias();
    virtual bool equivalent_to(std::shared_ptr<SymbolType> other);
    virtual std::string to_str();
};

class SymbolBoolean : public SymbolType {
   public:
    SymbolBoolean() : SymbolType("boolean") {}
};

class SymbolInteger : public SymbolType {
   public:
    SymbolInteger() : SymbolType("integer") {}
};

class SymbolDouble : public SymbolType {
   public:
    SymbolDouble() : SymbolType("double") {}
};

class SymbolChar : public SymbolType {
   public:
    SymbolChar() : SymbolType("char") {}
};

class SymbolString : public SymbolType {
   public:
    SymbolString() : SymbolType("string") {}
};

const auto SYMBOL_BOOLEAN = std::make_shared<SymbolBoolean>();
const auto SYMBOL_INTEGER = std::make_shared<SymbolInteger>();
const auto SYMBOL_DOUBLE = std::make_shared<SymbolDouble>();
const auto SYMBOL_CHAR = std::make_shared<SymbolChar>();
const auto SYMBOL_STRING = std::make_shared<SymbolString>();

class SymbolRecord : public SymbolType {
   public:
    explicit SymbolRecord(std::shared_ptr<SymbolTable> fields)
        : SymbolType("record"), fields(std::move(fields)) {}
    std::string get_type_of_object_str() override;
    bool equivalent_to(std::shared_ptr<SymbolType> other) override;
    std::string to_str() override;

   protected:
    std::shared_ptr<SymbolTable> fields;
};

class SymbolTypeAlias : public SymbolType {
   public:
    SymbolTypeAlias(std::string name, std::shared_ptr<SymbolType> original)
        : SymbolType(std::move(name)), original(std::move(original)) {}
    std::string get_type_of_object_str() override;
    SymbolType* resolve_alias() override;
    std::string to_str() override;

   protected:
    std::shared_ptr<SymbolType> original;
};
#endif
