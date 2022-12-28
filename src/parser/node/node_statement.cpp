#include "node_statement.h"

namespace parser {
void NodeCallStatement::draw(std::ostream& os, int depth) {
    func_call->draw(os, depth);
}

void NodeCompoundStatement::draw(std::ostream& os, int depth) {
    os << "compounds statements";
    draw_vector(os, depth + 1, stmts);
}

void NodeForStatement::draw(std::ostream& os, int depth) {
    os << "for\n";
    draw_path(os, depth + 1);
    param->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    start_exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    dir->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    end_exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    stmt->draw(os, depth + 1);
}

void NodeWhileStatement::draw(std::ostream& os, int depth) {
    os << "while\n";
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    stmt->draw(os, depth + 1);
}

void NodeIfStatement::draw(std::ostream& os, int depth) {
    os << "if\n";
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    stmt->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    os << "else:\n";
    if (else_stmt != nullptr) {
        draw_path(os, depth + 2);
        else_stmt->draw(os, depth + 1);
    } else {
        draw_path(os, depth + 2);
        os << "empty";
    }
}

void NodeAssigmentStatement::draw(std::ostream& os, int depth) {
    os << op.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    var_ref->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
}
};  // namespace parser
