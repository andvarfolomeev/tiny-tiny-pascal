#ifndef SYMBOL_PROCEDURE_H
#define SYMBOL_PROCEDURE_H

#include "../parser/node/node_statement.h"
#include "symbol.h"
#include "symbol_table.h"
#include "symbol_type.h"

class SymbolProcedure : public SymbolType {
   public:
    SymbolProcedure(std::string name, std::shared_ptr<SymbolTable> locals,
                    std::shared_ptr<parser::NodeCompoundStatement> body)
        : SymbolType(name),
          locals(std::move(locals)),
          body(std::move(body)),
          ret(nullptr) {}

    void set_body(std::shared_ptr<parser::NodeCompoundStatement> body_);
    std::string get_type_of_object_str() override;
    void draw_additional(std::ostream &os, int depth) override;
    bool is_standard_io();
    virtual bool is_procedure();
    virtual bool is_function();
    std::shared_ptr<SymbolTable> get_locals();
    std::shared_ptr<SymbolType> get_ret();
    unsigned int get_count_of_params();

   protected:
    std::shared_ptr<SymbolTable> locals;
    std::shared_ptr<parser::NodeCompoundStatement> body;
    std::shared_ptr<SymbolType> ret;
};

const auto SYMBOL_WRITELN =
    std::make_shared<SymbolProcedure>("writeln", nullptr, nullptr);
const auto SYMBOL_WRITE =
    std::make_shared<SymbolProcedure>("write", nullptr, nullptr);
const auto SYMBOL_READ =
    std::make_shared<SymbolProcedure>("read", nullptr, nullptr);

class SymbolFunction : public SymbolProcedure {
   public:
    SymbolFunction(std::string name, std::shared_ptr<SymbolTable> locals,
                   std::shared_ptr<parser::NodeCompoundStatement> body,
                   std::shared_ptr<SymbolType> ret)
        : SymbolProcedure(name, locals, body) {
        this->ret = ret;
    }

    void set_ret_type(std::shared_ptr<SymbolType> ret_);
    std::string get_type_of_object_str() override;
    std::string get_ret_type_str() override;
    bool is_procedure() override;
    bool is_function() override;
};

const auto SYMBOL_RANDOM = std::make_shared<SymbolFunction>(
    "random", std::make_shared<SymbolTable>(), nullptr, SYMBOL_DOUBLE);
const std::vector<std::shared_ptr<Symbol>> SQRT_ARGS{
    std::make_shared<SymbolParam>("x", SYMBOL_DOUBLE)};
const auto SYMBOL_SQRT = std::make_shared<SymbolFunction>(
    "sqrt", std::make_shared<SymbolTable>(SQRT_ARGS), nullptr, SYMBOL_DOUBLE);

#endif
