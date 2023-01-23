#ifndef SYMBOL_VAR_H
#define SYMBOL_VAR_H

#include "symbol.h"
#include "symbol_type.h"

namespace visitor {
class GeneratorVisitor;
class BaseVisitorWithResult;
}  // namespace visitor

class SymbolVar : public Symbol {
   public:
    SymbolVar(std::string name, std::shared_ptr<SymbolType> type)
        : Symbol(name), type(std::move(type)) {
        size = this->type->size;
    }
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
    std::shared_ptr<SymbolType> get_type();
    virtual bool is_param();
    virtual void accept(visitor::GeneratorVisitor *visitor);
    virtual void accept(visitor::BaseVisitorWithResult *visitor, bool result);

   protected:
    std::shared_ptr<SymbolType> type;
};

class SymbolVarLocal : public SymbolVar {
   public:
    SymbolVarLocal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
    void accept(visitor::GeneratorVisitor *visitor) override;
    void accept(visitor::BaseVisitorWithResult *visitor, bool result) override;
};

class SymbolVarGlobal : public SymbolVar {
   public:
    SymbolVarGlobal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVar(name, type) {}
    std::string get_type_of_object_str() override;
    void accept(visitor::GeneratorVisitor *visitor) override;
    void accept(visitor::BaseVisitorWithResult *visitor, bool result) override;
};

class SymbolConstLocal : public SymbolVarLocal {
   public:
    SymbolConstLocal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVarLocal(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolConstGlobal : public SymbolVarGlobal {
   public:
    SymbolConstGlobal(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVarGlobal(name, type) {}
    std::string get_type_of_object_str() override;
};

class SymbolParam : public SymbolVarLocal {
   public:
    SymbolParam(std::string name, std::shared_ptr<SymbolType> type)
        : SymbolVarLocal(name, type) {}
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
