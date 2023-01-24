#include "generator.h"

#include <algorithm>
#include <sstream>

#include "../symbol_table/symbol_type.h"
#include "../symbol_table/symbol_type_array.h"

Operand::Operand(OperandValue value)
    : value(std::move(value)), flags(0), offset(0) {
    if (get_if<SymbolVar>(&value)) set_flag(OperandFlag::VAR);
}
Operand Operand::operator&(OperandFlag flag) {
    set_flag(flag);
    return std::move(*this);
}
Operand Operand::operator+(int _offset) {
    offset = _offset;
    set_flag(OperandFlag::INDIRECT);
    return std::move(*this);
}
Operand Operand::operator-(int _offset) {
    offset = -_offset;
    set_flag(OperandFlag::INDIRECT);
    return std::move(*this);
}
std::string Operand::value_to_string(OperandValue value) {
    if (const Register *reg_val = std::get_if<Register>(&value)) {
        return std::string(magic_enum::enum_name<Register>(*reg_val));
    }
    if (const int *int_val = std::get_if<int>(&value)) {
        return std::to_string(*int_val);
    }
    if (const std::string *str_val = std::get_if<std::string>(&value)) {
        return *str_val;
    }
    const SymbolVar *sym_val = std::get_if<SymbolVar>(&value);
    return sym_val->get_name();
}
std::string Operand::to_string() {
    std::string result = value_to_string(value);
    if (has_flag(OperandFlag::VAR)) {
        result = "var_" + result;
    }
    if (has_flag(OperandFlag::PROCEDURE)) {
        result = "proc_" + result;
    }
    if (has_flag(OperandFlag::FUNCTION)) {
        result = "func_" + result;
    }
    if (has_flag(OperandFlag::QUOTED)) {
        result = "\"" + result + "\"";
    }
    if (offset != 0) {
        // save sign when offset is positive
        std::string number_with_sign =
            ((offset >= 0) ? "+" : "") + std::to_string(offset);
        result += number_with_sign;
    }
    if (has_flag(OperandFlag::INDIRECT)) {
        result = "[" + result + "]";
    }
    if (has_flag(OperandFlag::BYTE)) {
        result = "byte " + result;
    }
    if (has_flag(OperandFlag::WORD)) {
        result = "word " + result;
    }
    if (has_flag(OperandFlag::DWORD)) {
        result = "dword " + result;
    }
    if (has_flag(OperandFlag::QWORD)) {
        result = "qword " + result;
    }
    if (has_flag(OperandFlag::TWORD)) {
        result = "tword " + result;
    }
    return result;
}
bool Operand::has_flag(OperandFlag flag) const { return flags & int(flag); }
void Operand::set_flag(OperandFlag flag) { flags |= int(flag); }
Operand operator+(Register reg, int offset) {
    Operand op(reg);
    op = op + offset;
    return op;
}
Operand operator-(Register reg, int offset) {
    Operand op(reg);
    op = op - offset;
    return op;
}
Operand operator&(Register reg, OperandFlag flag) {
    Operand op(reg);
    op = op & flag;
    return op;
}
Operand operator&(int value, OperandFlag flag) {
    Operand op(value);
    op = op & flag;
    return op;
}
Operand operator&(std::string value, OperandFlag flag) {
    Operand op(value);
    op = op & flag;
    return op;
}
OperandFlag get_size_flag(int size) {
    switch (size) {
        case 1:
            return OperandFlag::BYTE;
        case 2:
            return OperandFlag::WORD;
        case 4:
            return OperandFlag::DWORD;
        default:
            return OperandFlag::QWORD;
    }
}
Command::Command(
    Instruction instruction,
    std::initializer_list<std::variant<Operand, OperandValue>> _operands)
    : instruction(instruction) {
    for (auto &operand : _operands) {
        if (const Operand *op_value = std::get_if<Operand>(&operand)) {
            operands.push_back(*op_value);
        } else {
            auto operand_value_value = std::get_if<OperandValue>(&operand);
            operands.emplace_back(*operand_value_value);
        }
    }
}
std::string Command::to_string() {
    std::ostringstream res;
    if (instruction == Instruction::COMMENT) {
        res << "\t; ";
        for (auto it = operands.begin(); it != operands.end(); ++it) {
            if (it != operands.begin()) {
                res << ", ";
            }
            res << it->to_string();
        }
        return res.str();
    }
    res << "\t" << magic_enum::enum_name<Instruction>(instruction);
    if (!operands.empty()) res << "\t";
    for (auto it = operands.begin(); it != operands.end(); ++it) {
        if (it != operands.begin()) {
            res << ", ";
        }
        res << it->to_string();
    }
    return res.str();
}

void Generator::gen(Section section, std::string label, Command command) {
    if (data.contains(section)) {
        data[section][label].push_back(command);
    } else {
        data[section][label] = std::vector<Command>();
        data[section][label].push_back(command);
    }
}

void Generator::gen(
    Section section, std::string label, Instruction code,
    std::initializer_list<std::variant<Operand, OperandValue>> args) {
    if (data.contains(section)) {
        data[section][label].emplace_back(code, args);
    } else {
        data[section][label] = std::vector<Command>();
        data[section][label].emplace_back(code, args);
    }
}

void Generator::gen(Command command) {
    gen(current_section, current_label, command);
}

void Generator::gen(
    Instruction code,
    std::initializer_list<std::variant<Operand, OperandValue>> args) {
    gen(current_section, current_label, code, args);
}

void Generator::gen_int_cmp(Instruction instruction) {
    gen(Instruction::XOR, {Register::ECX, Register::ECX});
    gen(Instruction::CMP, {Register::EAX, Register::EBX});
    gen(instruction, {Register::CL});
    gen(Instruction::MOV, {Register::EAX, Register::ECX});
}

void Generator::gen_double_cmp(Instruction instruction) {
    gen(Instruction::XOR, {Register::ECX, Register::ECX});
    gen(Instruction::COMISD, {Register::XMM0, Register::XMM1});
    gen(instruction, {Register::CL});
    gen(Instruction::MOV, {Register::EAX, Register::ECX});
}

void Generator::gen_push_double(Register reg) {
    gen(Instruction::SUB, {Register::ESP, 8});
    gen(Instruction::MOVSD,
        {Register::ESP & OperandFlag::INDIRECT & OperandFlag::QWORD, reg});
}

void Generator::gen_pop_double(Register reg) {
    gen(Instruction::MOVSD, {reg, Register::ESP + 0 & OperandFlag::QWORD});
    gen(Instruction::ADD, {Register::ESP, 8});
}

void Generator::set_section(Section section) { current_section = section; }
void Generator::set_label(std::string label) {
    current_label = label;
    if (!data[current_section].contains(label)) {
        data[current_section][current_label] = std::vector<Command>();
    }
}

void Generator::set(Section section, std::string label) {
    set_section(section);
    set_label(label);
}

std::string Generator::add_label(std::string title) {
    label_counter++;
    std::string label_name =
        "label_" + title + "_" + std::to_string(label_counter);
    return label_name;
}

std::string Generator::add_constant(int value) {
    constant_counter++;
    std::string label_name =
        "constant_value_" + std::to_string(constant_counter);
    gen(Section::DATA, label_name, Instruction::DD, {value});
    return label_name;
}

std::string Generator::add_constant(double value) {
    constant_counter++;
    std::string label_name =
        "constant_value_" + std::to_string(constant_counter);
    gen(Section::DATA, label_name, Instruction::DQ, {std::to_string(value)});
    return label_name;
}

std::string Generator::add_constant(DefaultConstant dc, int value) {
    std::string label_name = std::string(magic_enum::enum_name(dc));
    gen(Section::DATA, label_name, Instruction::DD, {std::to_string(value)});
    return label_name;
}

std::string Generator::add_constant(DefaultConstant dc, double value) {
    std::string label_name = std::string(magic_enum::enum_name(dc));
    gen(Section::DATA, label_name, Instruction::DQ, {std::to_string(value)});
    return label_name;
}

std::string Generator::add_constant(std::string value, bool newline) {
    constant_counter++;
    std::string label_name =
        "constant_value_" + std::to_string(constant_counter);
    if (newline) {
        gen(Section::DATA, label_name, Instruction::DB,
            {value & OperandFlag::QUOTED, 10, 0});
    } else {
        gen(Section::DATA, label_name, Instruction::DB,
            {value & OperandFlag::QUOTED, 0});
    }
    return label_name;
}

std::string Generator::add_constant(
    const std::vector<std::shared_ptr<parser::NodeExpression>> &params,
    bool newline) {
    std::stringstream value;
    for (auto &param : params) {
        if (param->sym_type->equivalent_to(SYMBOL_INTEGER)) {
            value << "%d";
        }
        if (param->sym_type->equivalent_to(SYMBOL_BOOLEAN)) {
            value << "%d";
        }
        if (param->sym_type->equivalent_to(SYMBOL_CHAR)) {
            value << "%c";
        }
        if (param->sym_type->equivalent_to(SYMBOL_DOUBLE)) {
            value << "%f";
        }
        if (param->sym_type->equivalent_to(SYMBOL_STRING)) {
            value << "%s";
        }
    }
    return add_constant(value.str(), newline);
}

std::string Generator::add_global_variable(SymbolVar *var) {
    std::string label_name = "var_" + var->get_name();
    if (std::dynamic_pointer_cast<SymbolArray>(var->get_type()) != nullptr) {
        gen(Section::BSS, label_name, Instruction::RESB, {"4"});
    } else if (auto rec =
                   std::dynamic_pointer_cast<SymbolRecord>(var->get_type())) {
        gen(Section::BSS, label_name, Instruction::RESB, {rec->size});
    } else if (var->get_type()->equivalent_to(SYMBOL_STRING)) {
        gen(Section::BSS, label_name, Instruction::RESB, {"4"});
    } else if (var->get_type()->equivalent_to(SYMBOL_DOUBLE)) {
        gen(Section::BSS, label_name, Instruction::RESQ, {"1"});
    } else {
        gen(Section::BSS, label_name, Instruction::RESD, {"1"});
    }
    return label_name;
}

std::string Generator::get(DefaultConstant c) {
    return std::string(magic_enum::enum_name(c));
}

void Generator::write(std::ostream &os, std::vector<Command> commands) {
    for (auto &command : commands) os << command.to_string() << "\n";
}

void Generator::write(std::ostream &os) {
    write(os, prolog);
    for (auto &[section_enum, section_label] : data) {
        std::string section = std::string(magic_enum::enum_name(section_enum));
        std::transform(section.begin(), section.end(), section.begin(),
                       ::tolower);
        os << "\n\tSECTION ." << section << "\n";
        for (auto &[label_name, label_commands] : section_label) {
            os << label_name << ":\n";
            write(os, label_commands);
        }
    }
}

Generator::Generator()
    : current_section(Section::TEXT), current_label("_main") {
    prolog.push_back(Command(Instruction::GLOBAL, {"_main"}));
    prolog.push_back(Command(Instruction::EXTERN, {"_printf"}));
    prolog.push_back(Command(Instruction::EXTERN, {"_scanf"}));
    prolog.push_back(Command(Instruction::EXTERN, {"_malloc"}));

    add_constant(DefaultConstant::TRUE, 1);
    add_constant(DefaultConstant::FALSE, 0);
    add_constant(DefaultConstant::DOUBLE_MINUS_ONE, -1.0);
}
