#include "symbol_function.h"

#include <utility>

void SymbolProcedure::set_body(
    std::shared_ptr<parser::NodeCompoundStatement> body_) {
    body = std::move(body_);
}

std::string SymbolProcedure::get_type_of_object_str() { return "procedure"; }

void SymbolProcedure::draw_additional(std::ostream& os, int depth) {
    Symbol::draw_additional(os, depth);
    if (locals != nullptr) locals->draw(os, depth);
}

void SymbolFunction::set_ret_type(std::shared_ptr<SymbolType> ret_) {
    ret = std::move(ret_);
}

std::string SymbolFunction::get_type_of_object_str() { return "function"; }

std::string SymbolFunction::get_ret_type_str() { return ret->to_str(); }
