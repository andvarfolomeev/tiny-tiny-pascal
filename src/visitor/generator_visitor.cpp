#include "generator_visitor.h"

#include <ranges>

#include "../parser/node/nodes.h"
#include "../symbol_table/symbol_function.h"

namespace visitor {
void GeneratorVisitor::visit(NodeKeyword* node) {}

// ok
void GeneratorVisitor::visit(NodeBlock* node) {
    for (auto& decl : node->declarations) {
        decl->accept(this);
    }
    node->compound_statement->accept(this);
}
void GeneratorVisitor::visit(NodeVarDecl* node) {}
void GeneratorVisitor::visit(NodeVarDecls* node) {}
void GeneratorVisitor::visit(NodeConstDecl* node) {}
void GeneratorVisitor::visit(NodeConstDecls* node) {}
void GeneratorVisitor::visit(NodeTypeDecl* node) {}
void GeneratorVisitor::visit(NodeTypeDecls* node) {}
void GeneratorVisitor::visit(NodeFormalParamSection* node) {}
void GeneratorVisitor::visit(NodeProcedureDecl* node) {}
void GeneratorVisitor::visit(NodeFunctionDecl* node) {}
void GeneratorVisitor::visit(NodeId* node) {}
void GeneratorVisitor::visit(NodeBinOp* node) {
    node->left->accept(this);
    node->right->accept(this);
    g.gen(Instruction::POP, {Register::EBX});  // right
    g.gen(Instruction::POP, {Register::EAX});  // left
    // TODO FLOAT
    switch (node->token.get_type()) {
        case TokenType::KEYWORD:
            switch (node->token.get_value<Keywords>()) {
                case scanner::Keywords::OR:
                    g.gen(Instruction::OR, {Register::EAX, Register::EBX});
                    break;
                case scanner::Keywords::AND:
                    g.gen(Instruction::AND, {Register::EAX, Register::EBX});
                    break;
                case scanner::Keywords::DIV:
                    g.gen(Instruction::CDQ, {});
                    g.gen(Instruction::IDIV, {Register::EBX});
                    break;
                case scanner::Keywords::MOD:
                    g.gen(Instruction::CDQ, {});
                    g.gen(Instruction::IDIV, {Register::EBX});
                    g.gen(Instruction::MOV, {Register::EAX, Register::EDX});
                    break;
                case scanner::Keywords::XOR:
                    g.gen(Instruction::XOR, {Register::EAX, Register::EBX});
                    break;
                case scanner::Keywords::SHR:
                    g.gen(Instruction::MOV, {Register::ECX, Register::EBX});
                    g.gen(Instruction::SHR, {Register::EAX, Register::CL});
                    break;
                case scanner::Keywords::SHL:
                    g.gen(Instruction::MOV, {Register::ECX, Register::EBX});
                    g.gen(Instruction::SHL, {Register::EAX, Register::CL});
                    break;
                default:
                    break;
            }
            break;
        case TokenType::OPER:
            switch (node->token.get_value<Operators>()) {
                case scanner::Operators::ADD:
                    g.gen(Instruction::ADD, {Register::EAX, Register::EBX});
                    break;
                case scanner::Operators::SUB:
                    g.gen(Instruction::SUB, {Register::EAX, Register::EBX});
                    break;
                case scanner::Operators::QUO:
                case scanner::Operators::MUL:
                    g.gen(Instruction::IMUL, {Register::EAX, Register::EBX});
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    g.gen(Instruction::PUSH, {Register::EAX});
}

void GeneratorVisitor::visit(NodeUnOp* node) {
    node->operand->accept(this);
    g.gen(Instruction::POP, {Register::EAX});
    if (node->token.is({TokenType::OPER})) {
        switch (node->token.get_value<Operators>()) {
            case Operators::ADD:
                // TODO: just skip????
                break;
            case Operators::SUB:
                g.gen(Instruction::NOT, {Register::EAX});
                g.gen(Instruction::ADD, {Register::EAX, 1});
                break;
        }
    } else {  // keyword not
        g.gen(Instruction::NOT, {Register::EAX});
    }
    g.gen(Instruction::PUSH, {Register::EAX});
}

// TODO:
void GeneratorVisitor::visit(NodeRelOp* node) {
    node->left->accept(this);
    node->right->accept(this);
    g.gen(Instruction::POP, {Register::EBX});
    g.gen(Instruction::POP, {Register::EAX});
    switch (node->token.get_value<Operators>()) {
        case Operators::EQL:
            g.gen_int_cmp(Instruction::SETE);
            break;
        case Operators::LES:
            g.gen_int_cmp(Instruction::SETL);
            break;
        case Operators::NEQ:
            g.gen_int_cmp(Instruction::SETNE);
            break;
        case Operators::LEQ:
            g.gen_int_cmp(Instruction::SETLE);
            break;
        case Operators::GTR:
            g.gen_int_cmp(Instruction::SETG);
            break;
        case Operators::GEQ:
            g.gen_int_cmp(Instruction::SETGE);
            break;
    }
    g.gen(Instruction::PUSH, {Register::EAX});
}
// ok
void GeneratorVisitor::visit(NodeBoolean* node) {
    g.gen(Instruction::PUSH,
          {node->token == Keywords::TRUE ? g.get(DefaultConstant::TRUE)
                                         : g.get(DefaultConstant::FALSE)});
}
// ok
void GeneratorVisitor::visit(NodeNumber* node) {
    int result = node->token.get_value<int>();
    g.gen(Instruction::PUSH, {result & OperandFlag::DWORD});
}
void GeneratorVisitor::visit(NodeCast* node) {
    // TODO:
}
// ok
void GeneratorVisitor::visit(NodeString* node) {
    g.gen(Instruction::PUSH,
          {g.add_constant(node->token.get_value<std::string>())});
}
void GeneratorVisitor::visit(NodeFuncCall* node) {
    for (auto& params : std::views::reverse(node->params)) params->accept(this);
    auto sym_proc = std::dynamic_pointer_cast<SymbolProcedure>(
        node->var_ref->get_sym_type());
    if (sym_proc->is_standard_io()) {
        auto format_label_name =
            g.add_constant(node->params, sym_proc->is_writeln_proc());
        g.gen(Instruction::PUSH, {format_label_name});
        if (sym_proc->is_writeln_proc() || sym_proc->is_write_proc()) {
            g.gen(Instruction::CALL, {"_printf"});
        }
        g.gen(Instruction::ADD, {Register::ESP, 4});

        return;
    }
}
void GeneratorVisitor::visit(NodeArrayAccess* node) {}
void GeneratorVisitor::visit(NodeRecordAccess* node) {}
void GeneratorVisitor::visit(NodeProgram* node) {
    node->block->accept(this);
    g.gen(Instruction::RET, {});
}
void GeneratorVisitor::visit(NodeCallStatement* node) {
    node->func_call->accept(this);
}
// ok
void GeneratorVisitor::visit(NodeCompoundStatement* node) {
    for (auto& statement : node->stmts) statement->accept(this);
}
void GeneratorVisitor::visit(NodeForStatement* node) {}
void GeneratorVisitor::visit(NodeWhileStatement* node) {}
void GeneratorVisitor::visit(NodeIfStatement* node) {}
void GeneratorVisitor::visit(NodeAssigmentStatement* node) {}
void GeneratorVisitor::visit(NodeSimpleType* node) {}
void GeneratorVisitor::visit(NodeRange* node) {}
void GeneratorVisitor::visit(NodeArrayType* node) {}
void GeneratorVisitor::visit(NodeFieldSelection* node) {}
void GeneratorVisitor::visit(NodeRecordType* node) {}
}  // namespace visitor