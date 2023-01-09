#include "node_expression.h"

namespace parser {

std::string NodeId::get_name() { return token.get_value<std::string>(); }
Token& NodeId::get_token() { return token; }
Position NodeId::get_pos() { return token.get_pos(); }

Position NodeBoolean::get_pos() { return token.get_pos(); }

std::shared_ptr<NodeExpression> NodeBinOp::get_left() { return left; }
std::shared_ptr<NodeExpression> NodeBinOp::get_right() { return right; }
Position NodeBinOp::get_pos() { return token.get_pos(); }

Position NodeUnOp::get_pos() { return token.get_pos(); }

Position NodeRelOp::get_pos() { return token.get_pos(); }

Position NodeNumber::get_pos() { return token.get_pos(); }

Position NodeString::get_pos() { return token.get_pos(); }

std::shared_ptr<NodeVarRef> NodeFuncCall::get_var_ref() { return var_ref; }

std::vector<std::shared_ptr<NodeExpression>> NodeFuncCall::get_params() {
    return params;
}
Position NodeFuncCall::get_pos() { return var_ref->get_pos(); }

Position NodeArrayAccess::get_pos() { return var_ref->get_pos(); }

Position NodeRecordAccess::get_pos() { return field->get_pos(); }

std::shared_ptr<NodeExpression> NodeCast::get_expression() { return exp; }
Position NodeCast::get_pos() { return exp->get_pos(); }
};  // namespace parser
