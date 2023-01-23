#include "generator_visitor.h"

#include <ranges>

#include "../parser/node/nodes.h"
#include "../symbol_table/symbol_function.h"

namespace visitor {
void GeneratorVisitor::visit(NodeKeyword* node, bool result) {}

// ok
void GeneratorVisitor::visit(NodeBlock* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeBlock"});
    for (auto& decl : node->declarations) {
        decl->accept(this, false);
    }
    node->compound_statement->accept(this, false);
}

void GeneratorVisitor::visit(SymbolVar* sym, bool result) {}
void GeneratorVisitor::visit(SymbolVarLocal* sym, bool result) {
    g.gen(Instruction::COMMENT, {"START SymbolVarLocal"});
    auto label = g.add_global_variable(sym);
    // TODO: expression
}
void GeneratorVisitor::visit(SymbolVarGlobal* sym, bool result) {
    g.gen(Instruction::COMMENT, {"SymbolVarGlobal"});
    auto label = g.add_global_variable(sym);
    // TODO: expression
}

void GeneratorVisitor::visit(NodeVarDecl* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeVarDecl"});
    for (auto& sym : node->syms) {
        sym->accept(this, false);
    }
    if (node->exp != nullptr) {
        node->exp->accept(this, false);
        if (node->syms[0]->get_type()->equivalent_to(SYMBOL_DOUBLE)) {
            g.gen_pop_double(Register::XMM0);
            g.gen(Instruction::MOVSD,
                  {node->syms[0]->get_name() & OperandFlag::QWORD &
                       OperandFlag::INDIRECT & OperandFlag::VAR,
                   Register::XMM0});
            return;
        }
        g.gen(Instruction::POP, {node->syms[0]->get_name() & OperandFlag::VAR &
                                 OperandFlag::INDIRECT & OperandFlag::DWORD});
    }
}
void GeneratorVisitor::visit(NodeVarDecls* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeVarDecls"});
    for (auto& var : node->vars) {
        var->accept(this, false);
    }
}
void GeneratorVisitor::visit(NodeConstDecl* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeConstDecl"});
    node->sym->accept(this, false);
}
void GeneratorVisitor::visit(NodeConstDecls* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeConstDecls"});
    for (auto& var : node->consts) {
        var->accept(this, false);
    }
}
void GeneratorVisitor::visit(NodeTypeDecl* node, bool result) {}
void GeneratorVisitor::visit(NodeTypeDecls* node, bool result) {}
void GeneratorVisitor::visit(NodeFormalParamSection* node, bool result) {}
void GeneratorVisitor::visit(NodeProcedureDecl* node, bool result) {}
void GeneratorVisitor::visit(NodeFunctionDecl* node, bool result) {}

// ok
void GeneratorVisitor::visit(NodeId* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeId"});
    auto var = node->get_name() & OperandFlag::VAR;

    if (node->sym_type->equivalent_to(SYMBOL_DOUBLE)) {
        g.gen(Instruction::MOV, {Register::EAX, var});
        if (result) {
            g.gen(Instruction::MOVSD,
                  {Register::XMM0, Register::EAX + 0 & OperandFlag::QWORD});
            g.gen_push_double(Register::XMM0);
        } else {
            g.gen(Instruction::PUSH, {Register::EAX});
        }
        return;
    }

    if (node->sym_type->equivalent_to(SYMBOL_STRING)) {
        if (result) {
            g.gen(Instruction::PUSH, {var & OperandFlag::INDIRECT &
                                      get_size_flag(node->sym_type->size)});
        } else {
            g.gen(Instruction::PUSH, {var});
        }
    }

    if (node->sym_type->equivalent_to(SYMBOL_INTEGER)) {
        if (result) {
            g.gen(Instruction::PUSH, {var & OperandFlag::INDIRECT &
                                      get_size_flag(node->sym_type->size)});
        } else {
            g.gen(Instruction::PUSH, {var});
        }
    }
}

// ok
void GeneratorVisitor::visit(NodeBinOp* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeBinOp"});
    node->left->accept(this, true);
    node->right->accept(this, true);
    if (node->sym_type->equivalent_to(SYMBOL_INTEGER)) {
        g.gen(Instruction::POP, {Register::EBX});  // right
        g.gen(Instruction::POP, {Register::EAX});  // left
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
                        g.gen(Instruction::IMUL,
                              {Register::EAX, Register::EBX});
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    } else {
        g.gen(
            Instruction::MOVSD,
            {Register::XMM1, Register::ESP + 8 & OperandFlag::QWORD});  // left
        g.gen(
            Instruction::MOVSD,
            {Register::XMM0, Register::ESP + 0 & OperandFlag::QWORD});  // right
        g.gen(Instruction::ADD, {Register::ESP, 16});
        switch (node->token.get_value<Operators>()) {
            case scanner::Operators::ADD:
                g.gen(Instruction::ADDSD, {Register::XMM1, Register::XMM0});
                break;
            case scanner::Operators::SUB:
                g.gen(Instruction::SUBSD, {Register::XMM1, Register::XMM0});
                break;
            case scanner::Operators::QUO:
                g.gen(Instruction::DIVSD, {Register::XMM1, Register::XMM0});
                break;
            case scanner::Operators::MUL:
                g.gen(Instruction::MULSD, {Register::XMM1, Register::XMM0});
                break;
            default:
                break;
        }
        g.gen(Instruction::SUB, {Register::ESP, 8});
        g.gen(Instruction::MOVSD,
              {Register::ESP + 0 & OperandFlag::QWORD, Register::XMM1});
        return;
    }
    g.gen(Instruction::PUSH, {Register::EAX});
}

// ok
void GeneratorVisitor::visit(NodeUnOp* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeUnOp"});
    node->operand->accept(this, true);
    if (node->sym_type->equivalent_to(SYMBOL_INTEGER) ||
        node->sym_type->equivalent_to(SYMBOL_BOOLEAN)) {
        g.gen(Instruction::POP, {Register::EAX});
        if (node->token.is({TokenType::OPER})) {
            switch (node->token.get_value<Operators>()) {
                case Operators::ADD:
                    break;
                case Operators::SUB:
                    g.gen(Instruction::NOT, {Register::EAX});
                    g.gen(Instruction::ADD, {Register::EAX, 1});
                    break;
                default:
                    break;
            }
        } else {  // keyword not
            g.gen(Instruction::NOT, {Register::EAX});
        }
        g.gen(Instruction::PUSH, {Register::EAX});
    } else {  // double
        g.gen_pop_double(Register::XMM0);
        switch (node->token.get_value<Operators>()) {
            case Operators::ADD:
                break;
            case Operators::SUB:
                g.gen(Instruction::MULSD,
                      {Register::XMM0,
                       g.get(DefaultConstant::DOUBLE_MINUS_ONE) &
                           OperandFlag::INDIRECT & OperandFlag::QWORD});
                break;
            default:
                break;
        }
        g.gen_push_double(Register::XMM0);
    }
}

// ok
void GeneratorVisitor::visit(NodeRelOp* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeRelOp"});
    node->left->accept(this, true);
    node->right->accept(this, true);
    if (node->left->sym_type->equivalent_to(SYMBOL_INTEGER)) {
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
    } else {  // double
        g.gen(
            Instruction::MOVSD,
            {Register::XMM1, Register::ESP + 8 & OperandFlag::QWORD});  // left
        g.gen(
            Instruction::MOVSD,
            {Register::XMM0, Register::ESP + 0 & OperandFlag::QWORD});  // right
        g.gen(Instruction::ADD, {Register::ESP, 16});
        switch (node->token.get_value<Operators>()) {
            case Operators::EQL:
                g.gen_double_cmp(Instruction::SETE);
                break;
            case Operators::LES:
                g.gen_double_cmp(Instruction::SETA);
                break;
            case Operators::NEQ:
                g.gen_double_cmp(Instruction::SETNE);
                break;
            case Operators::LEQ:
                g.gen_double_cmp(Instruction::SETAE);
                break;
            case Operators::GTR:
                g.gen_double_cmp(Instruction::SETB);
                break;
            case Operators::GEQ:
                g.gen_double_cmp(Instruction::SETBE);
                break;
        }
        g.gen(Instruction::PUSH, {Register::EAX});
    }
}

// ok
void GeneratorVisitor::visit(NodeBoolean* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeBoolean"});
    g.gen(Instruction::PUSH, {node->token == Keywords::TRUE});
}

// ok
void GeneratorVisitor::visit(NodeNumber* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeNumber"});
    if (node->token == TokenType::LITERAL_INTEGER) {
        int result = node->token.get_value<int>();
        g.gen(Instruction::PUSH, {result & OperandFlag::DWORD});
    } else {  // double
        auto label = g.add_constant(node->token.get_value<double>());
        g.gen(Instruction::MOVSD,
              {Register::XMM0,
               label & OperandFlag::INDIRECT & OperandFlag::QWORD});
        g.gen(Instruction::SUB, {Register::ESP, 8});
        g.gen(Instruction::MOVSD,
              {Register::ESP & OperandFlag::INDIRECT & OperandFlag::QWORD,
               Register::XMM0});
    }
}

// ok
void GeneratorVisitor::visit(NodeCast* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeCast"});
    node->exp->accept(this, true);
    // cast int to double
    // SemanticVisitor only adds an integer to double cast, so there are no
    // extra checks
    g.gen(Instruction::POP, {Register::EAX});
    g.gen(Instruction::CVTSI2SD, {Register::XMM0, Register::EAX});
    g.gen_push_double(Register::XMM0);
}

// ok
void GeneratorVisitor::visit(NodeString* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeString"});
    g.gen(Instruction::PUSH,
          {g.add_constant(node->token.get_value<std::string>())});
}

// TODO:
void GeneratorVisitor::visit(NodeFuncCall* node, bool result) {
    g.gen(Instruction::COMMENT, {"FuncCall"});
    for (auto& params : std::views::reverse(node->params))
        params->accept(this, true);  // TODO: result for var
    auto sym_proc = std::dynamic_pointer_cast<SymbolProcedure>(
        node->var_ref->get_sym_type());
    if (sym_proc->is_standard_io()) {
        auto format_label_name =
            g.add_constant(node->params, sym_proc->is_writeln_proc());
        g.gen(Instruction::PUSH, {format_label_name});
        if (sym_proc->is_writeln_proc() || sym_proc->is_write_proc()) {
            g.gen(Instruction::CALL, {"_printf"});
        }
        int offset = 0;
        for (auto& params : node->params) offset += params->sym_type->size;
        g.gen(Instruction::ADD, {Register::ESP, offset + 4});

        return;
    }
}
void GeneratorVisitor::visit(NodeArrayAccess* node, bool result) {}
void GeneratorVisitor::visit(NodeRecordAccess* node, bool result) {}

// ok
void GeneratorVisitor::visit(NodeProgram* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeProgram"});
    node->block->accept(this, result);
    g.gen(Instruction::MOV, {Register::EAX, 0});
    g.gen(Instruction::RET, {});
}

// ok
void GeneratorVisitor::visit(NodeCallStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeCallStatement"});
    node->func_call->accept(this, result);
}

// ok
void GeneratorVisitor::visit(NodeCompoundStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeCompoundStatement"});
    for (auto& statement : node->stmts) statement->accept(this, result);
}

// ok
void GeneratorVisitor::visit(NodeForStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeForStatement"});
    auto label_for_start = g.add_label("for_start");
    auto label_for_end = g.add_label("for_end");

    node->param->accept(this, false);
    node->start_exp->accept(this, true);

    // assign
    g.gen(Instruction::POP, {Register::EAX});
    g.gen(Instruction::POP, {Register::EBX});
    g.gen(Instruction::MOV,
          {Register::EBX & OperandFlag::INDIRECT, Register::EAX});

    g.set_label(label_for_start);
    node->param->accept(this, true);
    node->end_exp->accept(this, true);

    g.gen(Instruction::POP, {Register::EBX});  // end_exp
    g.gen(Instruction::POP, {Register::EAX});  // param

    if (node->dir->token == Keywords::TO) {
        g.gen_int_cmp(Instruction::SETLE);
    } else {
        g.gen_int_cmp(Instruction::SETGE);
    }
    g.gen(Instruction::CMP, {Register::EAX, 0});
    g.gen(Instruction::JE, {label_for_end});
    g.gen(Instruction::CMP, {Register::EAX, 0});
    g.gen(Instruction::JE, {label_for_end});

    node->stmt->accept(this, true);

    node->param->accept(this, false);
    g.gen(Instruction::POP, {Register::EAX});

    if (node->dir->token == Keywords::TO) {
        g.gen(Instruction::ADD,
              {Register::EAX & OperandFlag::INDIRECT, 1 & OperandFlag::DWORD});
    } else {
        g.gen(Instruction::SUB,
              {Register::EAX & OperandFlag::INDIRECT, 1 & OperandFlag::DWORD});
    }

    g.gen(Instruction::JMP, {label_for_start});
    g.set_label(label_for_end);
}

// ok
void GeneratorVisitor::visit(NodeWhileStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeWhileStatement"});
    auto label_while_start = g.add_label("while_start");
    auto label_while_end = g.add_label("while_end");
    g.set_label(label_while_start);

    node->exp->accept(this, true);
    g.gen(Instruction::POP, {Register::EAX});
    g.gen(Instruction::CMP, {Register::EAX, 0});
    g.gen(Instruction::JE, {label_while_end});

    node->stmt->accept(this, true);
    g.gen(Instruction::JMP, {label_while_start});
    g.set_label(label_while_end);
}

// ok
void GeneratorVisitor::visit(NodeIfStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeIfStatement"});
    node->exp->accept(this, true);
    auto label_if = g.add_label("if");
    auto label_else = g.add_label("else");
    auto label_if_end = g.add_label("if_end");
    g.gen(Instruction::POP, {Register::EAX});
    g.gen(Instruction::CMP, {Register::EAX, 0});
    if (node->else_stmt != nullptr) {
        g.gen(Instruction::JE, {label_else});
    } else {
        g.gen(Instruction::JE, {label_if_end});
    }
    node->stmt->accept(this, true);
    g.gen(Instruction::JMP, {label_if_end});
    if (node->else_stmt != nullptr) {
        g.set_label(label_else);
        node->else_stmt->accept(this, true);
    }
    g.set_label(label_if_end);
}

// ok
void GeneratorVisitor::visit(NodeAssigmentStatement* node, bool result) {
    g.gen(Instruction::COMMENT, {"START NodeAssigmentStatement"});
    node->var_ref->accept(this, false);
    node->exp->accept(this, true);
    if (node->var_ref->sym_type->equivalent_to(SYMBOL_DOUBLE)) {
        node->var_ref->accept(this, true);
        g.gen_pop_double(Register::XMM0);
        g.gen_pop_double(Register::XMM1);

        switch (node->op.get_value<Operators>()) {
            case Operators::ASSIGN:
                g.gen(Instruction::MOVSD, {Register::XMM0, Register::XMM1});
                break;
            case Operators::ADDASSIGN:
                g.gen(Instruction::ADDSD, {Register::XMM0, Register::XMM1});
                break;
            case Operators::SUBASSIGN:
                g.gen(Instruction::SUBSD, {Register::XMM0, Register::XMM1});
                break;
            case Operators::MULASSIGN:
                g.gen(Instruction::MULSD, {Register::XMM0, Register::XMM1});
                break;
            case Operators::QUOASSIGN:
                g.gen(Instruction::DIVSD, {Register::XMM0, Register::XMM1});
                break;
        }
        g.gen(Instruction::POP, {Register::EAX});
        g.gen(Instruction::MOVSD,
              {Register::EAX & OperandFlag::QWORD & OperandFlag::INDIRECT,
               Register::XMM0});
        return;
    }

    g.gen(Instruction::POP, {Register::ECX});
    g.gen(Instruction::POP, {Register::EBX});

    switch (node->op.get_value<Operators>()) {
        case Operators::ASSIGN:
            g.gen(Instruction::MOV,
                  {Register::EBX & OperandFlag::INDIRECT, Register::ECX});
            break;
        case Operators::ADDASSIGN:
            g.gen(Instruction::ADD,
                  {Register::EBX & OperandFlag::INDIRECT, Register::ECX});
            break;
        case Operators::SUBASSIGN:
            g.gen(Instruction::SUB,
                  {Register::EBX & OperandFlag::INDIRECT, Register::ECX});
            break;
        case Operators::MULASSIGN:
            g.gen(Instruction::MOV,
                  {Register::EAX, Register::EBX & OperandFlag::INDIRECT});
            g.gen(Instruction::IMUL, {Register::ECX});
            g.gen(Instruction::MOV,
                  {Register::EBX & OperandFlag::INDIRECT, Register::EAX});
            break;
        default:
            break;
    }
}
void GeneratorVisitor::visit(NodeSimpleType* node, bool result) {}
void GeneratorVisitor::visit(NodeRange* node, bool result) {}
void GeneratorVisitor::visit(NodeArrayType* node, bool result) {}
void GeneratorVisitor::visit(NodeFieldSelection* node, bool result) {}
void GeneratorVisitor::visit(NodeRecordType* node, bool result) {}
}  // namespace visitor