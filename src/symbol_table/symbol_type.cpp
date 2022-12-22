#include "symbol_type.h"

std::string SymbolType::get_type_of_object_str() { return "type"; }
std::string SymbolType::get_ret_type_str() { return name; }
std::string SymbolTypeAlias::get_type_of_object_str() { return "type alias"; }
std::string SymbolRecord::get_type_of_object_str() { return "record"; }
