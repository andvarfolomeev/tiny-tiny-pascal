#ifndef TINY_TINY_PASCAL_GENERATOR_H
#define TINY_TINY_PASCAL_GENERATOR_H

#include <magic_enum.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../parser/node/node_base_expression.h"
#include "../symbol_table/symbol_var.h"

enum class DefaultConstant { TRUE, FALSE };

enum class Section { TEXT, DATA };

enum class Register {
    EAX,
    EBX,
    ECX,
    EDX,
    ESI,
    EDI,
    ESP,
    EBP,
    CL,
    XMM0,
    XMM1,
    XMM2,
    XMM3,
    DB
};

enum class Instruction {
    DB,
    DD,

    ADD,
    SUB,
    IMUL,
    IDIV,

    SHL,
    SHR,

    AND,
    OR,
    XOR,

    CMP,
    SETL,
    SETG,
    SETLE,
    SETGE,
    SETE,
    SETNE,

    NOT,

    CDQ,

    PUSH,
    POP,
    MOV,
    CALL,
    RET,
    GLOBAL,
    EXTERN,
    SECTION
};

using OperandValue = std::variant<Register, int, std::string, SymbolVar>;

enum class OperandFlag {
    INDIRECT = 1,
    VAR = 1 << 1,
    PROCEDURE = 1 << 2,
    FUNCTION = 1 << 3,
    BYTE = 1 << 4,
    WORD = 1 << 5,
    DWORD = 1 << 6,
    QWORD = 1 << 7,
    TWORD = 1 << 8,
    QUOTED = 1 << 9
};

class Operand {
   public:
    explicit Operand(OperandValue value);
    Operand operator&(OperandFlag flag);
    Operand operator+(int _offset);
    Operand operator-(int _offset);
    static std::string value_to_string(OperandValue value);
    std::string to_string();
    [[nodiscard]] bool has_flag(OperandFlag flag) const;
    void set_flag(OperandFlag flag);

   protected:
    OperandValue value;
    int flags;
    int offset;
};

Operand operator+(Register reg, int offset);
Operand operator-(Register reg, int offset);
Operand operator&(Register reg, OperandFlag flag);
Operand operator&(int value, OperandFlag flag);
Operand operator&(std::string value, OperandFlag flag);

class Command {
   public:
    Command(
        Instruction instruction,
        std::initializer_list<std::variant<Operand, OperandValue>> _operands);

    std::string to_string();

   protected:
    Instruction instruction;
    std::vector<Operand> operands;
};

class Generator {
   public:
    Generator();

    void gen(Section section, std::string label, Command command);
    void gen(Section section, std::string label, Instruction code,
             std::initializer_list<std::variant<Operand, OperandValue>> args);

    void gen(Command command);
    void gen(Instruction code,
             std::initializer_list<std::variant<Operand, OperandValue>> args);

    void gen_int_cmp(Instruction instruction);

    void set_section(Section section);

    void set_label(std::string label);

    void set(Section section, std::string label);

    std::string add_constant(int value);

    std::string add_constant(std::string value, bool newline = false);
    std::string add_constant(
        const std::vector<std::shared_ptr<parser::NodeExpression>> &params,
        bool newline = false);

    Operand get(DefaultConstant c) { return default_constants.at(c); }

    void write(std::ostream &os, std::vector<Command> commands);
    void write(std::ostream &os);

    std::vector<Command> prolog;
    Section current_section;
    std::string current_label;

    int constant_counter = 0;

    // DefaultConstant, constant label
    std::map<DefaultConstant, Operand> default_constants;

    // Section, Label, std::vector<Command>
    std::map<Section, std::map<std::string, std::vector<Command>>> data;
};

#endif  // TINY_TINY_PASCAL_GENERATOR_H
