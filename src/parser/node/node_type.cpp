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

void NodeRange::draw(std::ostream& os, int depth) {
    os << "range:\n";
    draw_path(os, depth + 1);
    exp1->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    exp2->draw(os, depth + 1);
}

void NodeArrayType::draw(std::ostream& os, int depth) {
    os << "array type\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    draw_path(os, depth + 1);
    draw_vector(os, depth + 1, ranges);
}

void NodeFieldSelection::draw(std::ostream& os, int depth) {
    os << "fields\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    draw_vector(os, depth + 1, idents);
}

void NodeRecordType::draw(std::ostream& os, int depth) {
    os << "record";
    draw_vector(os, depth + 1, fields);
}
};  // namespace parser
