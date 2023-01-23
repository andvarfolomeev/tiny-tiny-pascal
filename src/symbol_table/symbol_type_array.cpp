#include "symbol_type_array.h"

#include <sstream>

#include "../parser/node/node_base_expression.h"

bool SymbolArray::equivalent_to(std::shared_ptr<SymbolType> other) {
    auto other_arr = std::dynamic_pointer_cast<SymbolArray>(other);
    if (other_arr == nullptr) return false;
    return type->equivalent_to(other_arr->type);
}
std::string SymbolArray::get_type_of_object_str() {
    return SymbolType::get_type_of_object_str();
}
std::string SymbolArray::to_str() {
    std::ostringstream os;
    os << "array of " << type->to_str();
    return os.str();
}
std::shared_ptr<SymbolType> SymbolArray::get_inner_type() { return type; }
std::pair<std::shared_ptr<parser::NodeExpression>,
          std::shared_ptr<parser::NodeExpression>>
SymbolArray::get_bounds() {
    return {low, high};
}
