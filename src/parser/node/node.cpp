#include "node.h"

namespace parser {
void SyntaxNode::draw_path(std::ostream& os, int depth) {
    for (auto i = 0; i < depth; ++i) os << "   ";
}

void NodeKeyword::draw(std::ostream& os, [[maybe_unused]] int depth) {
    os << token.get_raw_value();
}
Token NodeWithStringToken::get_token() { return token; }
std::string NodeKeyword::get_name() {
    auto buffer_in_lower = token.get_raw_value();
    std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                   buffer_in_lower.begin(), ::tolower);
    return buffer_in_lower;
}
};  // namespace parser
