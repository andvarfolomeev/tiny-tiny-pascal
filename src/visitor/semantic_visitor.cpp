#include "semantic_visitor.h"

#include <ranges>
#include <sstream>

#include "../parser/node/nodes.h"
#include "../symbol_table/symbol_function.h"

namespace visitor {
using namespace parser;

void SemanticVisitor::visit([[maybe_unused]] NodeKeyword *node) {}
void SemanticVisitor::visit(NodeBlock *node) {
    for (auto &decl : node->declarations) decl->accept(this);
    node->compound_statement->accept(this);
}
void SemanticVisitor::visit(NodeVarDecl *node) {
    for (auto &id : node->ids) {
        sym_table_stack->push(
            id, std::make_shared<SymbolVar>(id->get_name(),
                                            get_symbol_type(node->type)));
    }
    node->type->accept(this);
}

void SemanticVisitor::visit(NodeVarDecls *node) {
    for (auto &var_decl : node->vars) var_decl->accept(this);
}

void SemanticVisitor::visit(NodeConstDecl *node) {
    if (node->exp != nullptr) {
        node->exp->accept(this);
        check_type_exist(node->exp);
    }
    auto sym_type = (node->type != nullptr) ? get_symbol_type(node->type)
                                            : node->exp->get_sym_type();
    sym_table_stack->push(
        node->id, std::make_shared<SymbolVar>(node->id->get_name(), sym_type));
}

void SemanticVisitor::visit(NodeConstDecls *node) {
    for (auto &const_decl : node->consts) const_decl->accept(this);
}

void SemanticVisitor::visit(NodeTypeDecl *node) {
    node->type->accept(this);
    sym_table_stack->push(
        node->id, std::make_shared<SymbolTypeAlias>(
                      node->id->get_name(), get_symbol_type(node->type)));
}

void SemanticVisitor::visit(NodeTypeDecls *node) {
    for (auto &type_decl : node->types) type_decl->accept(this);
}

void SemanticVisitor::visit(NodeFormalParamSection *node) {
    auto modifier = node->get_modifier();
    auto sym_type = get_symbol_type(node->get_type());

    for (auto &id : node->get_ids()) {
        std::shared_ptr<SymbolVar> symbol_param;
        if (modifier != nullptr) {
            auto token = modifier->get_token();
            if (token == Keywords::CONST) {
                symbol_param = std::make_shared<SymbolConstParam>(
                    id->get_name(), sym_type);
            } else {
                symbol_param =
                    std::make_shared<SymbolVarParam>(id->get_name(), sym_type);
            }
        } else {
            symbol_param =
                std::make_shared<SymbolParam>(id->get_name(), sym_type);
        }
        node->type->accept(this);
        sym_table_stack->push(id, symbol_param);
    }
}

void SemanticVisitor::visit(NodeProcedureDecl *node) {
    auto local_table = std::make_shared<SymbolTable>();
    auto symbol_proc = std::make_shared<SymbolProcedure>(
        node->id->get_name(), local_table, node->block->compound_statement);
    sym_table_stack->push(local_table);
    for (auto &param : node->params) param->accept(this);
    node->block->accept(this);
    sym_table_stack->pop();
    sym_table_stack->push(node->id, symbol_proc);
}

void SemanticVisitor::visit(NodeFunctionDecl *node) {
    auto local_table = std::make_shared<SymbolTable>();
    auto symbol_func = std::make_shared<SymbolFunction>(
        node->id->get_name(), local_table, node->block->compound_statement,
        nullptr);
    // adding a function to a table provides a check that the function name
    // does not match the parameter names and local declarations
    local_table->push(symbol_func);
    sym_table_stack->push(local_table);
    for (auto &param : node->params) param->accept(this);
    node->type->accept(this);
    auto sym_func_type = get_symbol_type(node->type);
    symbol_func->set_ret_type(sym_func_type);
    node->block->accept(this);
    local_table->del(node->id->get_name());
    sym_table_stack->pop();
    sym_table_stack->push(node->id, symbol_func);
}
/**
 * call only in expressions
 * @param node
 */
void SemanticVisitor::visit(NodeId *node) {
    node->sym_type =
        get_symbol_type_by_id(node->get_name());  // maybe check functions
}

void SemanticVisitor::visit(NodeBoolean *node) {
    node->sym_type = SYMBOL_BOOLEAN;
    node->is_lvalue = false;
}

void SemanticVisitor::visit(NodeBinOp *node) {
    if (node->sym_type != nullptr) return;

    node->left->accept(this);
    node->right->accept(this);

    check_type_exist(node->left);
    check_type_exist(node->right);

    auto left_st = node->left->get_sym_type();
    auto right_st = node->right->get_sym_type();

    switch (node->token.get_type()) {
        case TokenType::KEYWORD:
            switch (node->token.get_value<Keywords>()) {
                case scanner::Keywords::OR:
                case scanner::Keywords::AND:
                    if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
                        node->sym_type = SYMBOL_INTEGER;
                        return;
                    }
                    if (equivalent(SYMBOL_BOOLEAN, left_st, right_st)) {
                        node->sym_type = SYMBOL_BOOLEAN;
                        return;
                    }
                    break;
                case scanner::Keywords::DIV:
                case scanner::Keywords::MOD:
                case scanner::Keywords::XOR:
                case scanner::Keywords::SHR:
                case scanner::Keywords::SHL:
                    if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
                        node->sym_type = SYMBOL_INTEGER;
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        case TokenType::OPER:
            switch (node->token.get_value<Operators>()) {
                case scanner::Operators::ADD:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        node->sym_type = solve_casting(node);
                        return;
                    }
                    if (equivalent(SYMBOL_STRING, left_st, right_st)) {
                        node->sym_type = SYMBOL_STRING;
                        return;
                    }
                    break;
                case scanner::Operators::SUB:
                case scanner::Operators::QUO:
                case scanner::Operators::MUL:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        node->sym_type = solve_casting(node);
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    throw make_exc<SemanticException>(node->token.get_pos())
        << "Operator is not overloaded " << left_st->to_str() << " "
        << node->token.get_raw_value() << " " << right_st->to_str()
        << make_exc_end;
}

void SemanticVisitor::visit(NodeUnOp *node) {
    if (node->sym_type != nullptr) return;

    node->operand->accept(this);
    check_type_exist(node->operand);
    node->sym_type = SYMBOL_BOOLEAN;

    if (node->token.is({Operators::ADD, Operators::SUB})) {
        if (node->operand->get_sym_type()->equivalent_to(SYMBOL_INTEGER)) {
            node->sym_type = SYMBOL_INTEGER;
            return;
        }
        if (node->operand->get_sym_type()->equivalent_to(SYMBOL_DOUBLE)) {
            node->sym_type = SYMBOL_DOUBLE;
            return;
        }
    }
    if (node->token == Keywords::NOT) {
        if (node->operand->get_sym_type()->equivalent_to(SYMBOL_INTEGER)) {
            node->sym_type = SYMBOL_INTEGER;
            return;
        }
        if (node->operand->get_sym_type()->equivalent_to(SYMBOL_BOOLEAN)) {
            node->sym_type = SYMBOL_BOOLEAN;
            return;
        }
    }
    throw make_exc<SemanticException>(node->token.get_pos())
        << "Unary operator is not overloaded " << node->token.get_raw_value()
        << " " << node->operand->get_sym_type()->to_str() << make_exc_end;
}

void SemanticVisitor::visit(NodeRelOp *node) {
    if (node->sym_type != nullptr) return;

    node->left->accept(this);
    node->right->accept(this);
    check_type_exist(node->left);
    check_type_exist(node->right);
    auto left_st = node->left->get_sym_type();
    auto right_st = node->right->get_sym_type();
    node->sym_type = SYMBOL_BOOLEAN;

    if (((left_st->is_arithmetic() && right_st->is_arithmetic()) ||
         (equivalent(left_st, right_st, {SYMBOL_STRING, SYMBOL_BOOLEAN}))))
        return;

    throw make_exc<SemanticException>(node->token.get_pos())
        << "Operator is not overloaded " << left_st->to_str() << " "
        << node->token.get_raw_value() << " " << right_st->to_str()
        << make_exc_end;
}

void SemanticVisitor::visit(NodeNumber *node) {
    if (node->token == TokenType::LITERAL_INTEGER)
        node->sym_type = SYMBOL_INTEGER;
    else
        node->sym_type = SYMBOL_DOUBLE;
    node->is_lvalue = false;
}

void SemanticVisitor::visit([[maybe_unused]] NodeCast *node) {}

void SemanticVisitor::visit(NodeString *node) {
    node->sym_type = SYMBOL_STRING;
    node->is_lvalue = false;
}
void SemanticVisitor::visit(NodeFuncCall *node) {
    node->var_ref->accept(this);
    auto sym_type = node->var_ref->get_sym_type();
    auto sym_type_casted = std::dynamic_pointer_cast<SymbolProcedure>(sym_type);
    if (sym_type_casted == nullptr) {
        throw make_exc<SemanticException>()
            << "It's not callable" << make_exc_end;
    }
    if (sym_type_casted->is_function()) {
        node->sym_type = sym_type_casted->get_ret();
    }
    for (auto &param : node->params) param->accept(this);
    if (sym_type_casted->is_standard_io()) {
        for (auto &param : node->params) {
            if (!equivalent(param->get_sym_type(),
                            {SYMBOL_INTEGER, SYMBOL_BOOLEAN, SYMBOL_DOUBLE,
                             SYMBOL_STRING})) {
                throw make_exc<SemanticException>()
                    << "it's cant use in standard io functions" << make_exc_end;
            }
        }
    } else {
        auto table = sym_type_casted->get_locals();
        auto ordered_names = table->get_ordered_names();
        auto count_of_params = sym_type_casted->get_count_of_params();
        if (count_of_params != node->params.size()) {
            throw make_exc<SemanticException>()
                << "Expected " << count_of_params
                << " count of parameters, but taken " << node->params.size()
                << make_exc_end;
        }
        for (unsigned int i = 0; i < count_of_params; ++i) {
            auto sym = table->get(ordered_names[i]);
            auto sym_var = std::dynamic_pointer_cast<SymbolVar>(sym);
            // TODO: check var
            solve_casting(sym_var->get_type(), node->params[i]);
            if (!sym_var->get_type()->equivalent_to(
                    node->params[i]->get_sym_type())) {
                throw make_exc<SemanticException>()
                    << "Expected " << sym_var->get_type()->to_str()
                    << " type, but taken " << node->params[i]->get_sym_type()
                    << make_exc_end;
            }
        }
    }
    node->is_lvalue = false;
}

void SemanticVisitor::visit(NodeArrayAccess *node) {
    node->var_ref->accept(this);
    node->index->accept(this);
    check_type_exist(node->var_ref);
    check_type_exist(node->index);
    auto sym_type_casted =
        std::dynamic_pointer_cast<SymbolArray>(node->var_ref->get_sym_type());
    if (sym_type_casted == nullptr) {
        throw make_exc<SemanticException>() << "It's not array" << make_exc_end;
    }
    if (node->index->get_sym_type() != SYMBOL_INTEGER) {
        throw make_exc<SemanticException>()
            << "Expected integer expression in array access" << make_exc_end;
    }
    node->sym_type = sym_type_casted->get_inner_type();
    node->is_lvalue = node->var_ref->is_lvalue;
}

void SemanticVisitor::visit(NodeRecordAccess *node) {
    node->var_ref->accept(this);
    check_type_exist(node->var_ref);
    auto sym_type_casted =
        std::dynamic_pointer_cast<SymbolRecord>(node->var_ref->get_sym_type());
    if (sym_type_casted == nullptr) {
        throw make_exc<SemanticException>()
            << "It's not record" << make_exc_end;
    }
    auto fields = sym_type_casted->get_fields();
    auto field = std::dynamic_pointer_cast<SymbolVar>(
        fields->get(node->field->get_name()));
    if (field == nullptr) {
        throw make_exc<SemanticException>()
            << "It's not field of record" << make_exc_end;
    }
    node->sym_type = field->get_type();
    node->is_lvalue = node->var_ref->is_lvalue;
}
// TODO: not necessary
void SemanticVisitor::visit([[maybe_unused]] NodeSetElement *node) {}
// TODO: not necessary
void SemanticVisitor::visit([[maybe_unused]] NodeSetConstructor *node) {}

void SemanticVisitor::visit(NodeProgram *node) {
    sym_table_stack->alloc();
    node->block->accept(this);
}

void SemanticVisitor::visit(NodeCallStatement *node) {
    node->func_call->accept(this);
}

void SemanticVisitor::visit(NodeCompoundStatement *node) {
    for (auto &stmt : node->stmts) stmt->accept(this);
}

void SemanticVisitor::visit(NodeForStatement *node) {
    node->param->accept(this);
    if (!node->param->is_lvalue) {
        throw make_exc<SemanticException>(node->dir->token.get_pos())
            << "lvalue expected" << make_exc_end;
    }
    node->start_exp->accept(this);
    node->end_exp->accept(this);
    if (!node->start_exp->get_sym_type()->equivalent_to(SYMBOL_INTEGER) ||
        !node->end_exp->get_sym_type()->equivalent_to(SYMBOL_INTEGER)) {
        throw make_exc<SemanticException>(node->dir->token.get_pos())
            << "Ordinal expected in for range" << make_exc_end;
    }
    node->stmt->accept(this);
}

void SemanticVisitor::visit(NodeWhileStatement *node) {
    node->exp->accept(this);
    if (!node->exp->get_sym_type()->equivalent_to(SYMBOL_BOOLEAN))
        throw make_exc<SemanticException>()
            << "Boolean expected" << make_exc_end;
    node->stmt->accept(this);
}

void SemanticVisitor::visit(NodeIfStatement *node) {
    node->exp->accept(this);
    if (!node->exp->get_sym_type()->equivalent_to(SYMBOL_BOOLEAN))
        throw make_exc<SemanticException>()
            << "Boolean expected" << make_exc_end;
    node->stmt->accept(this);
}
void SemanticVisitor::visit(NodeAssigmentStatement *node) {
    node->var_ref->accept(this);
    if (!node->var_ref->is_lvalue) {
        throw make_exc<SemanticException>()
            << "lvalue expected" << make_exc_end;
    }
    node->exp->accept(this);

    check_type_exist(node->var_ref);
    check_type_exist(node->exp);
    auto left_sym_type = node->var_ref->get_sym_type();
    auto left_sym_type_func =
        std::dynamic_pointer_cast<SymbolProcedure>(left_sym_type);
    if (left_sym_type_func != nullptr) {
        if (left_sym_type_func->is_procedure()) {
            throw make_exc<SemanticException>()
                << "expression expected, but taken procedure" << make_exc_end;
        }
        left_sym_type = left_sym_type_func->get_ret();
    }
    solve_casting(left_sym_type, node->exp);
    auto right_sym_type = node->exp->get_sym_type();

    switch (node->op.get_value<Operators>()) {
        case Operators::ASSIGN:
            if (equivalent(left_sym_type, right_sym_type, SYMBOL_INTEGER) ||
                equivalent(left_sym_type, right_sym_type, SYMBOL_DOUBLE) ||
                equivalent(left_sym_type, right_sym_type, SYMBOL_STRING))
                return;
            break;
        case Operators::ADDASSIGN:
        case Operators::SUBASSIGN:
        case Operators::MULASSIGN:
        case Operators::QUOASSIGN:
            if (equivalent(left_sym_type, right_sym_type, SYMBOL_INTEGER) ||
                equivalent(left_sym_type, right_sym_type, SYMBOL_DOUBLE))
                return;
            break;
        default:
            break;
    }
    throw make_exc<SemanticException>(node->op.get_pos())
        << "expression expected, but taken procedure" << make_exc_end;
}
void SemanticVisitor::visit([[maybe_unused]] NodeSimpleType *node) {}

void SemanticVisitor::visit(NodeRange *node) {
    auto beg = node->get_beg_exp();
    auto end = node->get_end_exp();
    beg->accept(this);
    end->accept(this);
    if (!beg->get_sym_type()->equivalent_to(SYMBOL_INTEGER) ||
        !end->get_sym_type()->equivalent_to(SYMBOL_INTEGER)) {
        throw make_exc<SemanticException>()
            << "Ordinal expected in array type declaration" << make_exc_end;
    }
}
void SemanticVisitor::visit(NodeArrayType *node) {
    for (auto &range : node->ranges) {
        range->accept(this);
    }
}
void SemanticVisitor::visit(NodeFieldSelection *node) {
    node->type->accept(this);
}
void SemanticVisitor::visit(NodeRecordType *node) {
    for (auto &field : node->fields) {
        field->accept(this);
    }
}
std::shared_ptr<SymbolTableStack> SemanticVisitor::get_sym_table_stack() {
    return sym_table_stack;
}
std::shared_ptr<SymbolType> SemanticVisitor::get_symbol_type(
    const std::shared_ptr<NodeType> &type) {
    auto type_record = std::dynamic_pointer_cast<NodeRecordType>(type);
    if (type_record) return get_symbol_type(type_record);
    auto type_array = std::dynamic_pointer_cast<NodeArrayType>(type);
    if (type_array) return get_symbol_type(type_array);
    // builtin primitive type or alias
    auto sym_type = sym_table_stack->get(
        std::dynamic_pointer_cast<NodeSimpleType>(type)->get_name());
    if (sym_type == nullptr)
        throw make_exc<SemanticException>() << "Not found type" << make_exc_end;
    if (!sym_type->is_type())
        throw make_exc<SemanticException>()
            << "Found but is not type" << make_exc_end;
    return std::dynamic_pointer_cast<SymbolType>(sym_type);
}

std::shared_ptr<SymbolType> SemanticVisitor::get_symbol_type(
    const std::shared_ptr<NodeRecordType> &type) {
    auto fields = type->get_fields();
    auto table = std::make_shared<SymbolTable>();
    for (auto &field : fields) {
        auto sym_type = get_symbol_type(field->get_type());
        for (auto &id : field->get_idents()) {
            table->push(id->get_name(),
                        std::make_shared<SymbolVar>(id->get_name(), sym_type));
        }
    }
    return std::make_shared<SymbolRecord>(table);
}

std::shared_ptr<SymbolType> SemanticVisitor::get_symbol_type(
    const std::shared_ptr<NodeArrayType> &array_type) {
    auto sym_type = get_symbol_type(array_type->get_type());
    auto ranges_ = array_type->get_ranges();
    for (auto &range : std::views::reverse(ranges_)) {
        sym_type = std::make_shared<SymbolArray>(sym_type, range->get_beg_exp(),
                                                 range->get_end_exp());
    }
    return sym_type;
}
std::shared_ptr<SymbolType> SemanticVisitor::get_symbol_type_by_id(
    const std::string &&name) {
    auto sym = sym_table_stack->get(name);
    auto sym_var = std::dynamic_pointer_cast<SymbolVar>(sym);
    if (sym_var != nullptr) return sym_var->get_type();
    auto sym_proc = std::dynamic_pointer_cast<SymbolProcedure>(sym);
    if (sym_proc != nullptr) return sym_proc;
    throw SemanticException("It's not symbol var");
}
std::shared_ptr<SymbolType> SemanticVisitor::solve_casting(NodeBinOp *node) {
    auto left_st = node->left->get_sym_type();
    auto right_st = node->right->get_sym_type();
    // if one of the operands is an integer and a real
    if ((left_st->equivalent_to(SYMBOL_DOUBLE) &&
         right_st->equivalent_to(SYMBOL_INTEGER)) ||
        (left_st->equivalent_to(SYMBOL_INTEGER) &&
         right_st->equivalent_to(SYMBOL_DOUBLE))) {
        node->left = left_st->equivalent_to(SYMBOL_INTEGER)
                         ? std::make_shared<NodeCast>(node->left, SYMBOL_DOUBLE)
                         : node->left;
        node->right =
            right_st->equivalent_to(SYMBOL_INTEGER)
                ? std::make_shared<NodeCast>(node->right, SYMBOL_DOUBLE)
                : node->right;
        return node->left->get_sym_type();
    } else if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
        return SYMBOL_INTEGER;
    } else if (equivalent(SYMBOL_DOUBLE, left_st, right_st)) {
        return SYMBOL_DOUBLE;
    } else {
        return nullptr;
    }
}

void SemanticVisitor::solve_casting(std::shared_ptr<SymbolType> left_st,
                                    std::shared_ptr<NodeExpression> &right) {
    auto right_st = right->get_sym_type();
    right = (left_st->equivalent_to(SYMBOL_DOUBLE) &&
             right_st->equivalent_to(SYMBOL_INTEGER))
                ? std::make_shared<NodeCast>(right, SYMBOL_DOUBLE)
                : right;
}

void SemanticVisitor::check_type_exist(
    const std::shared_ptr<NodeExpression> &exp) {
    if (exp->get_sym_type() == nullptr) {
        throw make_exc<SemanticException>()
            << "it's not return value" << make_exc_end;
    }
}
}  // namespace visitor