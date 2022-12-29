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

bool SymbolProcedure::is_standard_io() {
    return equivalent_to(SYMBOL_WRITELN) || equivalent_to(SYMBOL_WRITE) ||
           equivalent_to(SYMBOL_READ);
}
bool SymbolProcedure::is_procedure() { return true; }
bool SymbolProcedure::is_function() { return false; }
std::shared_ptr<SymbolType> SymbolProcedure::get_ret() { return ret; }
std::shared_ptr<SymbolTable> SymbolProcedure::get_locals() { return locals; }
unsigned int SymbolProcedure::get_count_of_params() {
    unsigned int res = 0;
    for (auto& name : locals->get_ordered_names()) {
        auto sym = locals->get(name);
        auto sym_var = std::dynamic_pointer_cast<SymbolParam>(sym);
        if (sym_var == nullptr) return res;
        if (!sym_var->is_param()) {
            break;
        }
        res++;
    }
    return res;
}

void SymbolFunction::set_ret_type(std::shared_ptr<SymbolType> ret_) {
    ret = std::move(ret_);
}

std::string SymbolFunction::get_type_of_object_str() { return "function"; }

std::string SymbolFunction::get_ret_type_str() { return ret->to_str(); }
bool SymbolFunction::is_procedure() { return false; }
bool SymbolFunction::is_function() { return true; }
