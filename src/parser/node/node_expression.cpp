#include "node_expression.h"

namespace parser {

std::string NodeId::get_name() { return token.get_value<std::string>(); }
Token& NodeId::get_token() { return token; }
Position NodeId::get_pos() { return token.get_pos(); }

Position NodeBoolean::get_pos() { return token.get_pos(); }

std::shared_ptr<SymbolType> NodeBinOp::solve_casting() {
    auto left_st = left->get_sym_type();
    auto right_st = right->get_sym_type();
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
        return left->get_sym_type();
    } else if (equivalent(SYMBOL_INTEGER, left_st, right_st)) {
        return SYMBOL_INTEGER;
    } else if (equivalent(SYMBOL_DOUBLE, left_st, right_st)) {
        return SYMBOL_DOUBLE;
    } else {
        return nullptr;
    }
}

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
