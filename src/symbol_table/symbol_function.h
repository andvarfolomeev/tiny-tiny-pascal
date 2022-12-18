#ifndef SYMBOL_PROCEDURE_H
#define SYMBOL_PROCEDURE_H

#include "../parser/node/node_statement.h"
#include "symbol.h"
#include "symbol_table.h"
#include "symbol_types.h"

class SymbolProcedure : public Symbol {
   public:
    SymbolProcedure(std::string name, std::shared_ptr<SymbolTable> params,
                    std::shared_ptr<SymbolTable> locals,
                    std::shared_ptr<parser::NodeCompoundStatement> body)
        : Symbol(name),
          params(std::move(params)),
          locals(std::move(locals)),
          body(std::move(body)) {}

   protected:
    std::shared_ptr<SymbolTable> params;
    std::shared_ptr<SymbolTable> locals;
    std::shared_ptr<parser::NodeCompoundStatement> body;
};

class SymbolFunction : public SymbolProcedure {
   public:
    SymbolFunction(std::string name, std::shared_ptr<SymbolTable> params,
                   std::shared_ptr<SymbolTable> locals,
                   std::shared_ptr<parser::NodeCompoundStatement> body,
                   std::shared_ptr<SymbolType> ret)
        : SymbolProcedure(name, params, locals, body), ret(std::move(ret)) {}

   protected:
    std::shared_ptr<SymbolType> ret;
};

#endif
