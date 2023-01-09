#include "printer_visitor.h"

#include "../parser/node/nodes.h"
#include "magic_enum.hpp"

namespace visitor {
void PrinterVisitor::draw_path() {
    //    for (auto i = 0; i < depth; i++) {
    //        os << "   ";
    //    }
    os << std::string(depth * 3, ' ');
}
void PrinterVisitor::pre_visit() { ++depth; }
void PrinterVisitor::post_visit() { --depth; }
template <typename T>
void PrinterVisitor::draw_vector(const std::vector<std::shared_ptr<T>>& vec) {
    for (auto& item : vec) {
        item->accept(this);
    }
}
void PrinterVisitor::draw_string(std::string&& str) {
    draw_path();
    os << str << "\n";
}
void PrinterVisitor::draw_token(scanner::Token token) {
    draw_string(token.get_raw_value());
}
void PrinterVisitor::visit(NodeKeyword* node) {
    pre_visit();
    draw_path();
    os << node->get_name() << "\n";
    post_visit();
}
void PrinterVisitor::visit(NodeBlock* node) {
    pre_visit();
    draw_string("decls");
    draw_vector(node->declarations);
    draw_string("block");
    node->compound_statement->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeVarDecl* node) {
    pre_visit();
    draw_string("var");
    draw_vector(node->ids);
    node->type->accept(this);
    if (node->exp != nullptr) {
        node->exp->accept(this);
    } else {
        draw_string("without exp");
    }
    post_visit();
}
void PrinterVisitor::visit(NodeVarDecls* node) {
    pre_visit();
    draw_string("vars");
    draw_vector(node->vars);
    post_visit();
}
void PrinterVisitor::visit(NodeConstDecl* node) {
    pre_visit();
    draw_string("const");
    node->id->accept(this);
    if (node->type != nullptr) {
        node->type->accept(this);
    } else {
        draw_string("without type");
    }
    node->exp->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeConstDecls* node) {
    pre_visit();
    draw_string("consts");
    draw_vector(node->consts);
    post_visit();
}
void PrinterVisitor::visit(NodeTypeDecl* node) {
    pre_visit();
    draw_string("type def");
    node->id->accept(this);
    node->type->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeTypeDecls* node) {
    pre_visit();
    draw_string("def types");
    draw_vector(node->types);
    post_visit();
}
void PrinterVisitor::visit(NodeFormalParamSection* node) {
    pre_visit();
    draw_string("param");
    if (node->modifier != nullptr) {
        node->modifier->accept(this);
    } else {
        draw_string("without modifier");
    }
    draw_vector(node->ids);
    node->type->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeProcedureDecl* node) {
    pre_visit();
    draw_string("procedure decl");
    node->id->accept(this);
    draw_vector(node->params);
    node->block->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeFunctionDecl* node) {
    pre_visit();
    draw_string("function decl");
    node->id->accept(this);
    node->type->accept(this);
    draw_vector(node->params);
    node->block->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeId* node) {
    pre_visit();
    draw_string(node->get_name());
    post_visit();
}
void PrinterVisitor::visit(NodeBoolean* node) {
    pre_visit();
    draw_token(node->token);
    post_visit();
}
void PrinterVisitor::visit(NodeBinOp* node) {
    pre_visit();
    draw_token(node->token);
    node->left->accept(this);
    node->right->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeUnOp* node) {
    pre_visit();
    draw_token(node->token);
    node->operand->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeRelOp* node) {
    pre_visit();
    draw_token(node->token);
    node->left->accept(this);
    node->right->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeNumber* node) {
    pre_visit();
    draw_token(node->token);
    post_visit();
}
void PrinterVisitor::visit(NodeCast* node) {
    pre_visit();
    draw_string("cast");
    draw_path();
    os << node->sym_type->to_str() << "\n";
    node->exp->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeString* node) {
    pre_visit();
    draw_token(node->token);
    post_visit();
}
void PrinterVisitor::visit(NodeFuncCall* node) {
    pre_visit();
    draw_string("call");
    node->var_ref->accept(this);
    pre_visit();
    draw_vector(node->params);
    post_visit();
    post_visit();
}
void PrinterVisitor::visit(NodeArrayAccess* node) {
    pre_visit();
    draw_string("array access");
    node->var_ref->accept(this);
    node->index->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeRecordAccess* node) {
    pre_visit();
    draw_string("field access");
    node->var_ref->accept(this);
    node->field->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeProgram* node) {
    draw_string("main");
    if (node->name != nullptr) {
        node->name->accept(this);
    } else {
        pre_visit();
        draw_string("without name");
        post_visit();
    }
    node->block->accept(this);
}
void PrinterVisitor::visit(NodeCallStatement* node) {
    node->func_call->accept(this);
}
void PrinterVisitor::visit(NodeCompoundStatement* node) {
    pre_visit();
    draw_string("compounds statements");
    draw_vector(node->stmts);
    post_visit();
}
void PrinterVisitor::visit(NodeForStatement* node) {
    pre_visit();
    draw_string("for");
    node->param->accept(this);
    node->start_exp->accept(this);
    node->dir->accept(this);
    node->end_exp->accept(this);
    node->stmt->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeWhileStatement* node) {
    pre_visit();
    draw_string("while");
    node->exp->accept(this);
    node->stmt->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeIfStatement* node) {
    pre_visit();
    draw_string("if");
    node->exp->accept(this);
    node->stmt->accept(this);
    draw_string("else");
    if (node->else_stmt != nullptr) {
        node->else_stmt->accept(this);
    } else {
        pre_visit();
        draw_string("empty");
        post_visit();
    }
    post_visit();
}
void PrinterVisitor::visit(NodeAssigmentStatement* node) {
    pre_visit();
    draw_token(node->op);
    node->var_ref->accept(this);
    node->exp->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeSimpleType* node) {
    pre_visit();
    draw_string("type");
    node->id->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeRange* node) {
    pre_visit();
    draw_string("range");
    node->exp1->accept(this);
    node->exp2->accept(this);
    post_visit();
}
void PrinterVisitor::visit(NodeArrayType* node) {
    pre_visit();
    draw_string("array type");
    node->type->accept(this);
    draw_vector(node->ranges);
    post_visit();
}
void PrinterVisitor::visit(NodeFieldSelection* node) {
    pre_visit();
    draw_string("fields");
    node->type->accept(this);
    draw_vector(node->idents);
    post_visit();
}
void PrinterVisitor::visit(NodeRecordType* node) {
    pre_visit();
    draw_string("record");
    draw_vector(node->fields);
    post_visit();
}
}  // namespace visitor