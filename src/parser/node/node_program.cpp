#include "node_program.h"

namespace parser {
void NodeProgram::draw(std::ostream& os, int depth) {
    os << "main\n";
    draw_path(os, depth + 1);
    os << "program name:\n";
    draw_path(os, depth + 2);
    if (name != nullptr) {
        name->draw(os, depth + 2);
    } else {
        os << "without name";
    }
    os << "\n";
    draw_path(os, depth + 1);
    block->draw(os, depth + 1);
}
};  // namespace parser
