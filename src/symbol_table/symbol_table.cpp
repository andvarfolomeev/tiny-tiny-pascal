#include "symbol_table.h"
#include <stdexcept>

std::shared_ptr<Symbol> SymbolTable::at(std::string name) {
    if (data.contains(name)) {
        return data.at(name);
    }
    return nullptr;
}

void SymbolTable::del(std::string name) {
    data.erase(name);
    ordered_names.erase(
        std::find(ordered_names.begin(), ordered_names.end(), name));
}

void SymbolTable::push(std::string name, std::shared_ptr<Symbol> symbol) {
    if (data.contains(name)) {
        throw std::runtime_error("double declaration");
    }
    data[name] = symbol;
}
