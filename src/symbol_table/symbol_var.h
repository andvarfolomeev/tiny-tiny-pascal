#ifndef SYMBOL_VAR_H
#define SYMBOL_VAR_H

#include "symbol.h"
#include "symbol_type.h"

class SymbolVar : public Symbol {
   public:
    SymbolVar(std::string name, std::shared_ptr<SymbolType> type)
        : Symbol(name), type(std::move(type)) {
        offset = this->type->offset;
    }
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
    std::shared_ptr<SymbolType> get_type();
    virtual bool is_param();

   protected:
    std::shared_ptr<SymbolType> type;
};

class SymbolVarLocal : public SymbolVar {
   public:
    SymbolVarLocal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolVarGlobal : public SymbolVar {
   public:
    SymbolVarGlobal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolConstLocal : public SymbolVar {
   public:
    SymbolConstLocal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolConstGlobal : public SymbolVar {
   public:
    SymbolConstGlobal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
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

#endif
