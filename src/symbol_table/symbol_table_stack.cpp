#include "symbol_table_stack.h"

#include <ranges>

#include "symbol_table.h"

void SymbolTableStack::push(std::string name, std::shared_ptr<Symbol> value) {
    data.back()->push(name, value);
}

void SymbolTableStack::push(const std::shared_ptr<Symbol> &&value) {
    push(value->get_name(), value);
}

void SymbolTableStack::push(std::shared_ptr<SymbolTable> table) {
    data.push_back(table);
}

void SymbolTableStack::push(std::shared_ptr<parser::NodeId> id,
                            std::shared_ptr<Symbol> symbol) {
    data.back()->push(id, symbol);
}

void SymbolTableStack::pop() { data.pop_back(); }

std::shared_ptr<Symbol> SymbolTableStack::get(std::string name) {
    for (auto &i : std::views::reverse(data)) {
        auto symbol = i->get(name);
        if (symbol != nullptr) {
            return symbol;
        }
    }
    return nullptr;
}

std::shared_ptr<Symbol> SymbolTableStack::get_in_scope(std::string name) {
    return data.back()->get(name);
}

void SymbolTableStack::alloc() {
    data.push_back(std::make_shared<SymbolTable>());
}

void SymbolTableStack::alloc_with_builtin() {
    data.push_back(SymbolTable::get_with_builtin());
}

int SymbolTableStack::size() { return data.size(); }

void SymbolTableStack::draw(std::ostream &os) {
    for (auto table : data) {
        table->draw(os);
    }
}
