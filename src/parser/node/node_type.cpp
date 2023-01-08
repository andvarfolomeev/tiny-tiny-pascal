#include "node_type.h"

namespace parser {
void NodeType::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << "Type";
}

void NodeSimpleType::draw(std::ostream& os, int depth) {
    os << "type:\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
}

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

void NodeRange::draw(std::ostream& os, int depth) {
    os << "range:\n";
    draw_path(os, depth + 1);
    exp1->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    exp2->draw(os, depth + 1);
}
std::shared_ptr<NodeExpression> NodeRange::get_beg_exp() { return exp1; }
std::shared_ptr<NodeExpression> NodeRange::get_end_exp() { return exp2; }
Position NodeRange::get_pos() { return exp1->get_pos(); }

void NodeArrayType::draw(std::ostream& os, int depth) {
    os << "array type\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    draw_path(os, depth + 1);
    draw_vector(os, depth + 1, ranges);
}

std::shared_ptr<NodeType> NodeArrayType::get_type() { return type; }

std::vector<std::shared_ptr<NodeRange>> NodeArrayType::get_ranges() {
    return ranges;
}

void NodeFieldSelection::draw(std::ostream& os, int depth) {
    os << "fields\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    draw_vector(os, depth + 1, idents);
}

std::vector<std::shared_ptr<NodeId>> NodeFieldSelection::get_idents() {
    return idents;
}

std::shared_ptr<NodeType> NodeFieldSelection::get_type() { return type; }

void NodeRecordType::draw(std::ostream& os, int depth) {
    os << "record";
    draw_vector(os, depth + 1, fields);
}

std::vector<std::shared_ptr<NodeFieldSelection>> NodeRecordType::get_fields() {
    return fields;
}
};  // namespace parser
