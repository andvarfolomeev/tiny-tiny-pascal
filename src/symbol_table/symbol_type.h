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
    virtual bool is_arithmetic();
    virtual std::string to_str();
};

class SymbolBoolean : public SymbolType {
   public:
    SymbolBoolean() : SymbolType("boolean") { size = 4; }
};

class SymbolInteger : public SymbolType {
   public:
    SymbolInteger() : SymbolType("integer") { size = 4; }
    bool is_arithmetic() override;
};

class SymbolDouble : public SymbolType {
   public:
    SymbolDouble() : SymbolType("double") { size = 8; }
    bool is_arithmetic() override;
};

class SymbolChar : public SymbolType {
   public:
    SymbolChar() : SymbolType("char") { size = 4; }
};

class SymbolString : public SymbolType {
   public:
    SymbolString() : SymbolType("string") { size = 4; }
};

const auto SYMBOL_BOOLEAN = std::make_shared<SymbolBoolean>();
const auto SYMBOL_INTEGER = std::make_shared<SymbolInteger>();
const auto SYMBOL_DOUBLE = std::make_shared<SymbolDouble>();
const auto SYMBOL_CHAR = std::make_shared<SymbolChar>();
const auto SYMBOL_STRING = std::make_shared<SymbolString>();

class SymbolRecord : public SymbolType {
   public:
    explicit SymbolRecord(std::shared_ptr<SymbolTable> fields)
        : SymbolType("record"), fields(std::move(fields)) {
        size = 0;
        for (auto& field : *this->fields) {
            size += field.second->size;
            field.second->offset = size;
        }
    }
    std::string get_type_of_object_str() override;
    bool equivalent_to(std::shared_ptr<SymbolType> other) override;
    std::string to_str() override;
    std::shared_ptr<SymbolTable>& get_fields();

   protected:
    std::shared_ptr<SymbolTable> fields;
};

class SymbolTypeAlias : public SymbolType {
   public:
    SymbolTypeAlias(std::string name, std::shared_ptr<SymbolType> original)
        : SymbolType(std::move(name)), original(std::move(original)) {
        size = resolve_alias()->size;
    }
    std::string get_type_of_object_str() override;
    SymbolType* resolve_alias() override;
    std::string to_str() override;

   protected:
    std::shared_ptr<SymbolType> original;
};

bool equivalent(const std::shared_ptr<SymbolType>& type1,
                const std::shared_ptr<SymbolType>& type2,
                const std::shared_ptr<SymbolType>& type3);
bool equivalent(const std::shared_ptr<SymbolType>& type1,
                const std::shared_ptr<SymbolType>& type2,
                std::vector<std::shared_ptr<SymbolType>> types);
bool equivalent(const std::shared_ptr<SymbolType>& type,
                std::vector<std::shared_ptr<SymbolType>> types);
#endif
