#include "node_expression.h"

namespace parser {

std::string NodeId::get_name() { return token.get_value<std::string>(); }
Token& NodeId::get_token() { return token; }
Position NodeId::get_pos() { return token.get_pos(); }
Position NodeBoolean::get_pos() { return token.get_pos(); }
Position NodeBinOp::get_pos() { return token.get_pos(); }
Position NodeUnOp::get_pos() { return token.get_pos(); }
Position NodeRelOp::get_pos() { return token.get_pos(); }
Position NodeNumber::get_pos() { return token.get_pos(); }
Position NodeString::get_pos() { return token.get_pos(); }
Position NodeFuncCall::get_pos() { return var_ref->get_pos(); }
Position NodeArrayAccess::get_pos() { return var_ref->get_pos(); }
Position NodeRecordAccess::get_pos() { return field->get_pos(); }
Position NodeCast::get_pos() { return exp->get_pos(); }
};  // namespace parser
