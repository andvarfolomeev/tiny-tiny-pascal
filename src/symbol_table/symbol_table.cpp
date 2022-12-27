#include "symbol_table.h"

#include <iomanip>
#include <memory>
#include <stdexcept>
#include <numeric>

#include "symbol_function.h"
#include "symbol_type.h"

std::shared_ptr<Symbol> SymbolTable::get(std::string name) {
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
    ordered_names.push_back(name);
}

std::shared_ptr<SymbolTable> SymbolTable::get_with_builtin() {
    auto table = std::make_shared<SymbolTable>();
    table->push("boolean", SYMBOL_BOOLEAN);
    table->push("integer", SYMBOL_INTEGER);
    table->push("double", SYMBOL_DOUBLE);
    table->push("char", SYMBOL_CHAR);
    table->push("string", SYMBOL_STRING);
    table->push("writeln", SYMBOL_WRITELN);
    table->push("write", SYMBOL_WRITE);
    table->push("read", SYMBOL_READ);
    return table;
}

void SymbolTable::draw(std::ostream& os, int depth) {
    std::vector<int> widths = {30, 30, 90};
    auto widths_num = std::accumulate(widths.begin(), widths.end(), 0);
    std::string sep(widths_num, '-');
    std::string sep_(widths_num , '=');
    std::string sep__(widths_num , '~');
    auto vert_sep = '|';
    if (depth == 0) {
        os << sep << "\n"
           << vert_sep << std::setw(widths[0] - 1) << std::left  //
           << " id"                                              //
           << vert_sep << std::setw(widths[1] - 1) << std::left  //
           << " type of object"                                  //
           << vert_sep << std::setw(widths[2] - 2) << std::left  //
           << " ret type" << vert_sep << "\n"
           << sep << "\n";
    } else {
        os << sep_ << "\n";
    }
    for (auto& name : ordered_names) {
        auto sym = data[name];
        os << vert_sep << " " << std::setw(widths[0] - 2) << std::left  //
           << sym->get_name()                                           //
           << vert_sep << " " << std::setw(widths[1] - 2) << std::left  //
           << sym->get_type_of_object_str()                             //
           << vert_sep << " " << std::setw(widths[2] - 3) << std::left  //
           << sym->get_ret_type_str()                                  //
           << vert_sep << "\n";
        sym->draw_additional(os, depth + 1);
    }
    if (depth == 0) {
        os << sep << "\n";
    } else {
        os << sep__ << "\n";
    }
}

int SymbolTable::size() { return data.size(); }

std::map<std::string, std::shared_ptr<Symbol>>::iterator SymbolTable::begin() {
    return data.begin();
}

std::map<std::string, std::shared_ptr<Symbol>>::iterator SymbolTable::end() {
    return data.end();
}
