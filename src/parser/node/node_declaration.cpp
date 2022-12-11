#include "node_declaration.h"

namespace parser {
void NodeBlock::draw(std::ostream& os, int depth) {
    os << "decls:";
    draw_vector(os, depth + 1, declarations);
    os << "\n";
    draw_path(os, depth);
    os << "block:\n";
    draw_path(os, depth + 1);
    compound_statement->draw(os, depth + 1);
}

void NodeVarDecl::draw(std::ostream& os, int depth) {
    os << "var";
    draw_vector(os, depth + 1, ids);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    if (exp != nullptr) {
        exp->draw(os, depth + 1);
    } else {
        os << "without exp";
    }
}

void NodeVarDecls::draw(std::ostream& os, int depth) {
    os << "vars:";
    draw_vector(os, depth + 1, vars);
}

void NodeConstDecl::draw(std::ostream& os, int depth) {
    os << "const\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    if (type != nullptr) {
        type->draw(os, depth + 1);
        os << "\n";
    } else {
        os << "without type\n";
    }
    draw_path(os, depth + 1);
    exp->draw(os, depth + 1);
}

void NodeConstDecls::draw(std::ostream& os, int depth) {
    os << "consts:";
    draw_vector(os, depth + 1, consts);
}

void NodeTypeDecl::draw(std::ostream& os, int depth) {
    os << "type def\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
}

void NodeTypeDecls::draw(std::ostream& os, int depth) {
    os << "def types:";
    draw_vector(os, depth + 1, types);
}

void NodeFormalParamSection::draw(std::ostream& os, int depth) {
    os << "param\n";
    if (modifier != nullptr) {
        draw_path(os, depth + 1);
        modifier->draw(os, depth + 1);
    } else {
        draw_path(os, depth + 1);
        os << "without modifier";
    }
    draw_vector(os, depth + 1, ids);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
}

void NodeProcedureDecl::draw(std::ostream& os, int depth) {
    os << "procedure decl\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
    draw_vector(os, depth + 1, params);
    os << "\n";
    draw_path(os, depth + 1);
    block->draw(os, depth + 1);
}

void NodeFunctionDecl::draw(std::ostream& os, int depth) {
    os << "function decl\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
    draw_vector(os, depth + 1, params);
    os << "\n";
    draw_path(os, depth + 1);
    block->draw(os, depth + 1);
}

};  // namespace parser
