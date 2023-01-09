#include "node_type.h"

namespace parser {

std::string NodeSimpleType::get_name() {
    auto keyword = std::dynamic_pointer_cast<NodeKeyword>(id);
    if (keyword != nullptr) {
        return keyword->get_name();
    }
    return std::dynamic_pointer_cast<NodeId>(id)->get_name();
}

Position NodeSimpleType::get_pos() {
    auto keyword = std::dynamic_pointer_cast<NodeKeyword>(id);
    if (keyword != nullptr) {
        return keyword->get_token().get_pos();
    }
    return std::dynamic_pointer_cast<NodeId>(id)->get_pos();
}

std::shared_ptr<NodeExpression> NodeRange::get_beg_exp() { return exp1; }
std::shared_ptr<NodeExpression> NodeRange::get_end_exp() { return exp2; }
Position NodeRange::get_pos() { return exp1->get_pos(); }

std::shared_ptr<NodeType> NodeArrayType::get_type() { return type; }

std::vector<std::shared_ptr<NodeRange>> NodeArrayType::get_ranges() {
    return ranges;
}

std::vector<std::shared_ptr<NodeId>> NodeFieldSelection::get_idents() {
    return idents;
}

std::shared_ptr<NodeType> NodeFieldSelection::get_type() { return type; }

std::vector<std::shared_ptr<NodeFieldSelection>> NodeRecordType::get_fields() {
    return fields;
}
};  // namespace parser
