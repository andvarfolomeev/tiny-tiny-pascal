#include "symbol_var.h"

#include "../visitor/generator_visitor.h"

std::string SymbolVar::get_type_of_object_str() { return "var"; }
std::string SymbolVar::get_ret_type_str() { return type->to_str(); }
bool SymbolVar::is_param() { return false; }
std::string SymbolVarLocal::get_type_of_object_str() { return "local var"; }
void SymbolVarLocal::accept(visitor::GeneratorVisitor *visitor) {
    visitor->visit(this);
}
void SymbolVarLocal::accept(visitor::BaseVisitorWithResult *visitor,
                            bool result) {
    visitor->visit(this, result);
}
std::string SymbolVarGlobal::get_type_of_object_str() { return "global var"; }
void SymbolVarGlobal::accept(visitor::GeneratorVisitor *visitor) {
    visitor->visit(this);
}
void SymbolVarGlobal::accept(visitor::BaseVisitorWithResult *visitor,
                             bool result) {
    visitor->visit(this, result);
}
std::string SymbolVarParam::get_type_of_object_str() { return "var param"; }
std::string SymbolConstParam::get_type_of_object_str() { return "const param"; }
std::string SymbolParam::get_type_of_object_str() { return "param"; }
bool SymbolParam::is_param() { return true; }
std::string SymbolConstLocal::get_type_of_object_str() { return "local const"; }
std::string SymbolConstGlobal::get_type_of_object_str() {
    return "global const";
}
std::shared_ptr<SymbolType> SymbolVar::get_type() { return type; }
void SymbolVar::accept(visitor::GeneratorVisitor *visitor) {
    visitor->visit(this);
}
void SymbolVar::accept(visitor::BaseVisitorWithResult *visitor, bool result) {
    visitor->visit(this, result);
}
