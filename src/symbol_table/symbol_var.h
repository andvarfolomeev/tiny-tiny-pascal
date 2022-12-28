#ifndef SYMBOL_VAR_H
#define SYMBOL_VAR_H

#include "symbol.h"
#include "symbol_type.h"

class SymbolVar : public Symbol {
   public:
    SymbolVar(std::string name, std::shared_ptr<SymbolType> type)
        : Symbol(name), type(std::move(type)) {}
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
    std::shared_ptr<SymbolType> get_type();
    virtual bool is_param();

   protected:
    std::shared_ptr<SymbolType> type;
};

class SymbolParam : public SymbolVar {
   public:
    SymbolParam(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
    bool is_param() override;
};

class SymbolVarParam : public SymbolParam {
   public:
    SymbolVarParam(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolParam(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolConstParam : public SymbolParam {
   public:
    SymbolConstParam(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolParam(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolConst : public SymbolVar {
   public:
    SymbolConst(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
};

#endif
