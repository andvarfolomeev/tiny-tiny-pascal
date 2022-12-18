#include "node_expression.h"

namespace parser {
void NodeId::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value() << " ";
}

std::string NodeId::get_name() { return token.get_value<std::string>(); }

void NodeBinOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

void NodeUnOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    operand->draw(os, depth + 1);
}

void NodeRelOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

void NodeNumber::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

void NodeString::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}

void NodeFuncCall::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    draw_vector(os, depth + 1, params);
}

std::shared_ptr<NodeVarRef> NodeFuncCall::get_var_ref() { return var_ref; }
std::vector<std::shared_ptr<NodeExpression>> NodeFuncCall::get_params() {
    return params;
}

void NodeArrayAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    draw_vector(os, depth + 2, params);
}

void NodeRecordAccess::draw(std::ostream& os, int depth) {
    var_ref->draw(os, depth);
    os << "\n";
    draw_path(os, depth + 1);
    field->draw(os, depth + 1);
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
};  // namespace parser
