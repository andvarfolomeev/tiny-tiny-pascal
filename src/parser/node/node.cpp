#include "node.h"

namespace parser {
void SyntaxNode::draw_path(std::ostream& os, int depth) {
    for (auto i = 0; i < depth; ++i) os << "   ";
}

void NodeKeyword::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}
Token NodeWithStringToken::get_token() { return token; }
std::string NodeKeyword::get_name() { return token.get_value<std::string>(); }
};  // namespace parser
