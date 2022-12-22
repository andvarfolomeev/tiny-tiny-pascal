#include "node_expression.h"

namespace parser {
void NodeId::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value() << " ";
}

std::string NodeId::get_name() { return token.get_value<std::string>(); }

void NodeBoolean::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeBoolean::calc_sym_type() {
    return std::make_shared<SymbolBoolean>();
}

void NodeBinOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

std::shared_ptr<SymbolType> NodeBinOp::calc_sym_type() {
    // TODO:
    return NodeExpression::calc_sym_type();
}

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
    return std::make_shared<SymbolBoolean>();
}

void NodeNumber::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeNumber::calc_sym_type() {
    if (token == TokenType::LITERAL_DOUBLE) {
        return std::make_shared<SymbolDouble>();
    }
    return std::make_shared<SymbolInteger>();
}

void NodeString::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

std::shared_ptr<SymbolType> NodeString::calc_sym_type() {
    return std::make_shared<SymbolString>();
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
std::shared_ptr<NodeExpression> NodeCastToDecimal::get_expression() {
    return exp;
}
std::shared_ptr<SymbolType> NodeCastToDecimal::calc_sym_type() {
    return std::shared_ptr<SymbolDouble>();
}
};  // namespace parser
