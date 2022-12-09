#include "node.h"

#include <cstdlib>

namespace parser {
void SyntaxNode::draw_path(std::ostream& os, int depth) {
    for (auto i = 0; i < depth; ++i) os << "   ";
}

void SyntaxNode::draw_vector(
    std::ostream& os, int depth,
    std::shared_ptr<std::vector<std::shared_ptr<SyntaxNode>>> vec) {
    if (!vec) {
        os << "\n";
        draw_path(os, depth);
        os << "empty vector";
        return;
    }
    for (auto& item : *vec.get()) {
        os << "\n";
        draw_path(os, depth);
        item->draw(os, depth);
    }
}

void NodeSimpleType::draw(std::ostream& os, int depth) {
    os << "type:\n";
    draw_path(os, depth + 1);
    type_id->draw(os, depth + 1);
}

void NodeArrayIndexRange::draw(std::ostream& os, int depth) {
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
    draw_vector(os, depth + 1, range);
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

void NodeProgramBlock::draw(std::ostream& os, int depth) {
    os << "decls:";
    draw_vector(os, depth + 1, declarations);
    os << "\n";
    draw_path(os, depth);
    os << "block:\n";
    draw_path(os, depth + 1);
    compound_statement->draw(os, depth + 1);
}

void NodeVarDecls::draw(std::ostream& os, int depth) {
    os << "vars:";
    draw_vector(os, depth + 1, consts);
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

void NodeConstDecls::draw(std::ostream& os, int depth) {
    os << "consts:";
    draw_vector(os, depth + 1, consts);
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

void NodeTypeDecls::draw(std::ostream& os, int depth) {
    os << "def types:";
    draw_vector(os, depth + 1, types);
}

void NodeTypeDecl::draw(std::ostream& os, int depth) {
    os << "type def\n";
    draw_path(os, depth + 1);
    id->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
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
    draw_vector(os, depth + 1, idents);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
}

void NodeHeaderProcedureDecl::draw(std::ostream& os, int depth) {
    ident->draw(os, depth);
    draw_vector(os, depth + 1, params);
}

void NodeProcedureDecl::draw(std::ostream& os, int depth) {
    os << "procedure decl\n";
    draw_path(os, depth + 1);
    header->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    block->draw(os, depth + 1);
}

void NodeHeaderFunctionDecl::draw(std::ostream& os, int depth) {
    ident->draw(os, depth);
    draw_vector(os, depth + 1, params);
    os << "\n";
    draw_path(os, depth + 1);
    type->draw(os, depth + 1);
}

void NodeFunctionDecl::draw(std::ostream& os, int depth) {
    os << "function decl\n";
    draw_path(os, depth + 1);
    header->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    block->draw(os, depth + 1);
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
    op->draw(os, depth + 1);
}

void NodeWhileStatement::draw(std::ostream& os, int depth) {
    os << "while\n";
    draw_path(os, depth + 1);
    bool_exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    op->draw(os, depth + 1);
}

void NodeIfStatement::draw(std::ostream& os, int depth) {
    os << "if\n";
    draw_path(os, depth + 1);
    bool_exp->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    op->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    os << "else:\n";
    if (else_op != nullptr) {
        draw_path(os, depth + 2);
        else_op->draw(os, depth + 1);
    } else {
        draw_path(os, depth + 2);
        os << "empty";
    }
}

void NodeAssigmentStatement::draw(std::ostream& os, int depth) {
    os << op.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    exp1->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    exp2->draw(os, depth + 1);
}

void NodeType::draw(std::ostream& os, int depth) { os << "Type"; }

// void NodeExpression::draw(std::ostream& os, int depth) { os << "expression";
// }

void NodeBinOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

void NodeUnOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    operand->draw(os, depth + 1);
}

void NodeRelOp::draw(std::ostream& os, int depth) {
    os << token.get_raw_value() << "\n";
    draw_path(os, depth + 1);
    left->draw(os, depth + 1);
    os << "\n";
    draw_path(os, depth + 1);
    right->draw(os, depth + 1);
}

void NodeNumber::draw(std::ostream& os, int depth) {
    os << token.get_raw_value();
}

void NodeString::draw(std::ostream& os, int depth) {
    os << token.get_raw_value();
}

void NodeKeyword::draw(std::ostream& os, int depth) {
    os << token.get_raw_value();
}

void NodeId::draw(std::ostream& os, int depth) { os << token.get_raw_value() << " "; }

void NodeNil::draw(std::ostream& os, int depth) { os << token.get_raw_value(); }

void NodeFuncCall::draw(std::ostream& os, int depth) {
    id->draw(os, depth);
    draw_vector(os, depth + 1, params);
}

void NodeArrayAccess::draw(std::ostream& os, int depth) {
    id->draw(os, depth);
    draw_vector(os, depth + 2, params);
}

void NodeRecordAccess::draw(std::ostream& os, int depth) {
    id->draw(os, depth);
    os << "\n";
    draw_path(os, depth + 1);
    field->draw(os, depth + 1);
}

void NodeSetSimpleElement::draw(std::ostream& os, int depth) {
    exp->draw(os, depth);
}

void NodeSetRangeElement::draw(std::ostream& os, int depth) {
    exp1->draw(os, depth);
    os << "..";
    exp2->draw(os, depth);
}

void NodeSetConstructor::draw(std::ostream& os, int depth) {
    draw_vector(os, depth, set_elements);
}
}  // namespace parser
