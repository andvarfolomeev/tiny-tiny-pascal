#include "node_expression.h"

namespace parser {
void NodeId::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value() << " ";
}

std::string NodeId::get_name() { return token.get_value<std::string>(); }
std::shared_ptr<SymbolType> NodeId::calc_sym_type() { return sym_type; }

void NodeBoolean::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeBoolean::calc_sym_type() {
    return SYMBOL_BOOLEAN;
}

void NodeBinOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeBinOp::solve_casting() {
    auto left_st = left->calc_sym_type();
    auto right_st = right->calc_sym_type();
    // if one of the operands is an integer and a real
    if ((left_st->equivalent_to(SYMBOL_DOUBLE) &&
         right_st->equivalent_to(SYMBOL_INTEGER)) ||
        (left_st->equivalent_to(SYMBOL_INTEGER) &&
         right_st->equivalent_to(SYMBOL_DOUBLE))) {
        left = left_st->equivalent_to(SYMBOL_INTEGER)
                   ? std::make_shared<NodeCast>(left, SYMBOL_DOUBLE)
                   : left;
        right = right_st->equivalent_to(SYMBOL_INTEGER)
                    ? std::make_shared<NodeCast>(right, SYMBOL_DOUBLE)
                    : right;
        return left->calc_sym_type();
    } else if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
        return SYMBOL_INTEGER;
    } else if (equivalent(SYMBOL_DOUBLE, left_st, right_st)) {
        return SYMBOL_DOUBLE;
    } else {
        return nullptr;
    }
}

std::shared_ptr<SymbolType> NodeBinOp::calc_sym_type() {
    if (sym_type != nullptr) {
        return sym_type;
    }

    auto left_st = left->calc_sym_type();
    auto right_st = right->calc_sym_type();

    switch (token.get_type()) {
        case TokenType::KEYWORD:
            switch (token.get_value<Keywords>()) {
                case scanner::Keywords::OR:
                case scanner::Keywords::AND:
                    if (equivalent(SYMBOL_INTEGER, left_st, right_st))
                        sym_type = SYMBOL_INTEGER;
                    if (equivalent(SYMBOL_BOOLEAN, left_st, right_st))
                        sym_type = SYMBOL_BOOLEAN;
                    break;
                case scanner::Keywords::DIV:
                case scanner::Keywords::MOD:
                case scanner::Keywords::XOR:
                case scanner::Keywords::SHR:
                case scanner::Keywords::SHL:
                    if (equivalent(SYMBOL_INTEGER, left_st, right_st))
                        sym_type = SYMBOL_INTEGER;
                    break;
                default:
                    break;
            }
            break;
        case TokenType::OPER:
            switch (token.get_value<Operators>()) {
                case scanner::Operators::ADD:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        sym_type = solve_casting();
                    }
                    if (equivalent(SYMBOL_STRING, left_st, right_st))
                        sym_type = SYMBOL_STRING;
                    break;
                case scanner::Operators::SUB:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        sym_type = solve_casting();
                    }
                    break;
                case scanner::Operators::QUO:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        sym_type = solve_casting();
                    }
                    break;
                case scanner::Operators::MUL:
                    if (left_st->is_arithmetic() && right_st->is_arithmetic()) {
                        sym_type = solve_casting();
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return sym_type;
}

std::shared_ptr<NodeExpression> NodeBinOp::get_left() { return left; }
std::shared_ptr<NodeExpression> NodeBinOp::get_right() { return right; }

void NodeUnOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    operand->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeUnOp::calc_sym_type() {
    return operand->calc_sym_type();
}

void NodeRelOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeRelOp::calc_sym_type() {
    return SYMBOL_BOOLEAN;
}

void NodeNumber::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeNumber::calc_sym_type() {
    if (token == TokenType::LITERAL_DOUBLE) {
        return SYMBOL_DOUBLE;
    }
    return SYMBOL_INTEGER;
}

void NodeCast::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << "cast\n";
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
}

void NodeString::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeString::calc_sym_type() {
    return SYMBOL_STRING;
}

void NodeFuncCall::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    draw_vector(os, depth + 1, params);
}

std::shared_ptr<NodeVarRef> NodeFuncCall::get_var_ref() { return var_ref; }

std::vector<std::shared_ptr<NodeExpression>> NodeFuncCall::get_params() {
    return params;
}

std::shared_ptr<SymbolType> NodeFuncCall::calc_sym_type() {
    return NodeExpression::calc_sym_type();
}

void NodeArrayAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    draw_vector(os, depth + 2, params);
}

std::shared_ptr<SymbolType> NodeArrayAccess::calc_sym_type() {
    return NodeExpression::calc_sym_type();
}

void NodeRecordAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    os << "\n";
    draw_path(os, depth + 1);
    field->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeRecordAccess::calc_sym_type() {
    return field->calc_sym_type();
}

void NodeSetSimpleElement::draw(std::ostream& os, int depth) {
    exp->draw(os, depth);
}

void NodeSetRangeElement::draw(std::ostream& os, int depth) {
    exp1->draw(os, depth);
    os << "..";
    exp2->draw(os, depth);
}

void NodeSetConstructor::draw(std::ostream& os, int depth) {
    draw_vector(os, depth, elements);
}
std::shared_ptr<NodeExpression> NodeCast::get_expression() { return exp; }
std::shared_ptr<SymbolType> NodeCast::calc_sym_type() { return sym_type; }
};  // namespace parser
