#include "symbol_type.h"

#include <iostream>
#include <sstream>

#include "symbol_var.h"

std::string SymbolType::get_type_of_object_str() { return "type"; }
std::string SymbolType::get_ret_type_str() { return to_str(); }
SymbolType* SymbolType::resolve_alias() { return this; }
bool SymbolType::equivalent_to(std::shared_ptr<SymbolType> other) {
    return resolve_alias()->name == other->resolve_alias()->name;
}
bool SymbolType::is_arithmetic() { return false; }
std::string SymbolType::to_str() { return resolve_alias()->name; }
bool SymbolInteger::is_arithmetic() { return true; }
bool SymbolDouble::is_arithmetic() { return true; }
std::string SymbolTypeAlias::get_type_of_object_str() { return "type alias"; }
SymbolType* SymbolTypeAlias::resolve_alias() {
    return original->resolve_alias();
}
std::string SymbolTypeAlias::to_str() { return original->to_str(); }
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
std::string SymbolRecord::to_str() {
    std::ostringstream os;
    os << "record ";
    for (auto& it : *fields) {
        os << it.first << ": "
           << std::dynamic_pointer_cast<SymbolVar>(it.second)
                  ->get_type()
                  ->to_str()
           << "; ";
    }
    os << "end";
    return os.str();
}

bool equivalent(const std::shared_ptr<SymbolType>& type1,
                const std::shared_ptr<SymbolType>& type2,
                const std::shared_ptr<SymbolType>& type3) {
    return type1 == type2 && type2 == type3;
}
