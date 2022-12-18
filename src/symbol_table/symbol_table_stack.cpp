#include "symbol_table_stack.h"

#include <ranges>

void SymbolTableStack::push(std::string name, std::shared_ptr<Symbol> value) {
    data.back()->push(name, value);
}

std::shared_ptr<Symbol> SymbolTableStack::at(std::string name) {
    for (auto &i : std::views::reverse(data)) {
        auto symbol = i->at(name);
        if (symbol != nullptr) {
            return symbol;
        }
    }
    return nullptr;
}

void SymbolTableStack::alloc() {
    data.push_back(std::make_shared<SymbolTable>());
}
