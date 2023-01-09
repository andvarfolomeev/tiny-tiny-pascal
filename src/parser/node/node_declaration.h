#ifndef PARSER_NODE_DECLARATION_H
#define PARSER_NODE_DECLARATION_H

#include <vector>

#include "../../scanner/token.h"
#include "node.h"
#include "node_expression.h"
#include "node_statement.h"
#include "node_type.h"

namespace parser {
using namespace scanner;

class NodeDecl : public SyntaxNode {};

class NodeBlock : public NodeDecl {
   public:
    NodeBlock(std::vector<std::shared_ptr<NodeDecl>> declarations,
              std::shared_ptr<NodeCompoundStatement> compound_statement)
        : declarations(std::move(declarations)),
          compound_statement(std::move(compound_statement)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::vector<std::shared_ptr<NodeDecl>> declarations;
    std::shared_ptr<NodeCompoundStatement> compound_statement;
};

class NodeVarDecl : public NodeDecl {
   public:
    NodeVarDecl(std::vector<std::shared_ptr<NodeId>> ids,
                std::shared_ptr<NodeType> type,
                std::shared_ptr<NodeExpression> exp)
        : ids(std::move(ids)), type(std::move(type)), exp(std::move(exp)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::vector<std::shared_ptr<NodeId>> ids;
    std::shared_ptr<NodeType> type;
    std::shared_ptr<NodeExpression> exp;  // may be nullptr
};

class NodeVarDecls : public NodeDecl {
   public:
    explicit NodeVarDecls(std::vector<std::shared_ptr<NodeVarDecl>> vars)
        : vars(std::move(vars)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::vector<std::shared_ptr<NodeVarDecl>> vars;
};

class NodeConstDecl : public NodeDecl {
   public:
    NodeConstDecl(std::shared_ptr<NodeId> id, std::shared_ptr<NodeType> type,
                  std::shared_ptr<NodeExpression> exp)
        : id(std::move(id)), type(std::move(type)), exp(std::move(exp)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeId> id;
    std::shared_ptr<NodeType> type;  // may be nullptr
    std::shared_ptr<NodeExpression> exp;
};

class NodeConstDecls : public NodeDecl {
   public:
    explicit NodeConstDecls(std::vector<std::shared_ptr<NodeConstDecl>> consts)
        : consts(std::move(consts)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::vector<std::shared_ptr<NodeConstDecl>> consts;
};

class NodeTypeDecl : public NodeDecl {
   public:
    NodeTypeDecl(std::shared_ptr<NodeId> id, std::shared_ptr<NodeType> type)
        : id(std::move(id)), type(std::move(type)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeId> id;
    std::shared_ptr<NodeType> type;
};

class NodeTypeDecls : public NodeDecl {
   public:
    explicit NodeTypeDecls(std::vector<std::shared_ptr<NodeTypeDecl>> types)
        : types(std::move(types)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::vector<std::shared_ptr<NodeTypeDecl>> types;
};

class NodeFormalParamSection : public NodeDecl {
   public:
    NodeFormalParamSection(std::shared_ptr<NodeKeyword> modifier,
                           std::vector<std::shared_ptr<NodeId>> ids,
                           std::shared_ptr<NodeType> type)
        : modifier(std::move(modifier)),
          ids(std::move(ids)),
          type(std::move(type)) {}
    std::shared_ptr<NodeKeyword> get_modifier() { return modifier; }
    std::vector<std::shared_ptr<NodeId>> get_ids() { return ids; };
    std::shared_ptr<NodeType> get_type() { return type; };
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeKeyword> modifier;
    std::vector<std::shared_ptr<NodeId>> ids;
    std::shared_ptr<NodeType> type;
};

class NodeProcedureDecl : public NodeDecl {
   public:
    NodeProcedureDecl(
        std::shared_ptr<NodeId> id,
        std::vector<std::shared_ptr<NodeFormalParamSection>> params,
        std::shared_ptr<NodeBlock> block)
        : id(std::move(id)),
          params(std::move(params)),
          block(std::move(block)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeId> id;
    std::vector<std::shared_ptr<NodeFormalParamSection>> params;
    std::shared_ptr<NodeBlock> block;
};

class NodeFunctionDecl : public NodeProcedureDecl {
   public:
    NodeFunctionDecl(
        std::shared_ptr<NodeId> id,
        std::vector<std::shared_ptr<NodeFormalParamSection>> params,
        std::shared_ptr<NodeBlock> block, std::shared_ptr<NodeType> type)
        : NodeProcedureDecl(id, params, block), type(std::move(type)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeType> type;
};
}  // namespace parser

#endif  // PARSER_NODE_DECLARATION_H
