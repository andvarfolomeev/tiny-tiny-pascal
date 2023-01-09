#include "node_type.h"

namespace parser {

std::string NodeSimpleType::get_name() { return id->get_name(); }
Position NodeSimpleType::get_pos() { return id->get_pos(); }
Position NodeRange::get_pos() { return exp1->get_pos(); }
};  // namespace parser
