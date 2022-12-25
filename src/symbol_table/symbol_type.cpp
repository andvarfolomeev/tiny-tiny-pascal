#include "symbol_type.h"

std::string SymbolType::get_type_of_object_str() { return "type"; }
std::string SymbolType::get_ret_type_str() { return name; }
std::shared_ptr<SymbolType> SymbolType::resolve_alias() {
    return std::shared_ptr<SymbolType>(this);
}
std::string SymbolTypeAlias::get_type_of_object_str() { return "type alias"; }
std::shared_ptr<SymbolType> SymbolTypeAlias::resolve_alias() {
    return std::shared_ptr<SymbolType>(this->original);
}
std::string SymbolRecord::get_type_of_object_str() { return "record"; }
