#ifndef SYMBOL_PROCEDURE_H
#define SYMBOL_PROCEDURE_H

#include "../parser/node/node_statement.h"
#include "symbol.h"
#include "symbol_table.h"
#include "symbol_type.h"

class SymbolProcedure : public Symbol {
   public:
    SymbolProcedure(std::string name, std::shared_ptr<SymbolTable> locals,
                    std::shared_ptr<parser::NodeCompoundStatement> body)
        : Symbol(name), locals(std::move(locals)), body(std::move(body)) {}

    void set_body(std::shared_ptr<parser::NodeCompoundStatement> body_);
    std::string get_type_of_object_str() override;
    void draw_additional(std::ostream &os, int depth) override;

   protected:
    std::shared_ptr<SymbolTable> locals;
    std::shared_ptr<parser::NodeCompoundStatement> body;
};

class SymbolFunction : public SymbolProcedure {
   public:
    SymbolFunction(std::string name, std::shared_ptr<SymbolTable> locals,
                   std::shared_ptr<parser::NodeCompoundStatement> body,
                   std::shared_ptr<SymbolType> ret)
        : SymbolProcedure(name, locals, body), ret(std::move(ret)) {}

    void set_ret_type(std::shared_ptr<SymbolType> ret_);
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;

   protected:
    std::shared_ptr<SymbolType> ret;
};

#endif
