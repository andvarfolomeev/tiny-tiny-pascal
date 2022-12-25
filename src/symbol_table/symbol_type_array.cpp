#include "symbol_type_array.h"

bool SymbolArray::equivalent_to(std::shared_ptr<SymbolType> other) {
    auto other_arr = std::dynamic_pointer_cast<SymbolArray>(other);
    if (other_arr == nullptr) return false;
    if (ranges.size() != other_arr->ranges.size()) {
        return false;
    }
    return type->equivalent_to(other_arr->type);
}
std::string SymbolArray::get_type_of_object_str() {
    return SymbolType::get_type_of_object_str();
}
