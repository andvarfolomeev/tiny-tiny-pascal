#include "symbol_var.h"

std::string SymbolVar::get_type_of_object_str() { return "var"; }
std::string SymbolVar::get_ret_type_str() { return "some type"; }
std::string SymbolVarParam::get_type_of_object_str() { return "var param"; }
std::string SymbolParam::get_type_of_object_str() { return "param"; }
std::string SymbolConst::get_type_of_object_str() { return "const"; }
std::shared_ptr<SymbolType> SymbolVar::get_type() {
    return type;
}
