#include "symbol_type.h"

#include <iostream>

#include "symbol_var.h"

std::string SymbolType::get_type_of_object_str() { return "type"; }
std::string SymbolType::get_ret_type_str() { return name; }
SymbolType* SymbolType::resolve_alias() { return this; }
bool SymbolType::equivalent_to(std::shared_ptr<SymbolType> other) {
    return resolve_alias()->name == other->resolve_alias()->name;
}
std::string SymbolTypeAlias::get_type_of_object_str() { return "type alias"; }
SymbolType* SymbolTypeAlias::resolve_alias() {
    return original->resolve_alias();
}
std::string SymbolRecord::get_type_of_object_str() { return "record"; }
bool SymbolRecord::equivalent_to(std::shared_ptr<SymbolType> other) {
    auto other_rec = std::dynamic_pointer_cast<SymbolRecord>(other);
    if (other_rec == nullptr) return false;
    if (fields->size() != other_rec->fields->size()) return false;
    for (auto it = fields->begin(); it != fields->end(); ++it) {
        auto other_field = other_rec->fields->get(it->first);
        if (other_field == nullptr) return false;
        auto this_var = std::dynamic_pointer_cast<SymbolVar>(it->second);
        auto other_var = std::dynamic_pointer_cast<SymbolVar>(other_field);
        if (!this_var->get_type()->equivalent_to(other_var->get_type()))
            return false;
    }
    return true;
}
