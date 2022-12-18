#include "node.h"

namespace parser {
void SyntaxNode::draw_path(std::ostream& os, int depth) {
    for (auto i = 0; i < depth; ++i) os << "   ";
}

// template <typename T>
// void SyntaxNode::draw_vector(std::ostream& os, int depth,
//                              const std::vector<std::shared_ptr<T>>& vec) {}

void NodeKeyword::draw(std::ostream& os, int depth) {
    os << token.get_raw_value();
}
std::string NodeKeyword::get_name() {
    return token.get_value<std::string>();
}
};  // namespace parser
