#ifndef PARSER_NODE_TYPE_H
#define PARSER_NODE_TYPE_H

#include "../../scanner/token.h"
#include "node.h"
#include "node_expression.h"

namespace parser {
using namespace scanner;

class NodeType : public SyntaxNode {
   public:
    NodeType() {}
};

class NodeSimpleType : public NodeType {
   public:
    NodeSimpleType(std::shared_ptr<NodeId> id) : id(std::move(id)) {}
    std::string get_name();
    Position get_pos();
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    void accept(BaseVisitorWithResult *visitor, bool result) override {
        visitor->visit(this, result);
    }
    std::shared_ptr<NodeId> id;
};

class NodeRange : public NodeExpression {
   public:
    NodeRange(std::shared_ptr<NodeExpression> exp1,
              std::shared_ptr<NodeExpression> exp2)
        : exp1(std::move(exp1)), exp2(std::move(exp2)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    void accept(BaseVisitorWithResult *visitor, bool result) override {
        visitor->visit(this, result);
    }
    std::shared_ptr<NodeExpression> exp1;
    std::shared_ptr<NodeExpression> exp2;
};

class NodeArrayType : public NodeType {
   public:
    NodeArrayType(std::shared_ptr<NodeType> type,
                  std::vector<std::shared_ptr<NodeRange>> ranges)
        : type(std::move(type)), ranges(std::move(ranges)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    void accept(BaseVisitorWithResult *visitor, bool result) override {
        visitor->visit(this, result);
    }
    std::shared_ptr<NodeType> type;
    std::vector<std::shared_ptr<NodeRange>> ranges;
};

class NodeFieldSelection : public SyntaxNode {
   public:
    NodeFieldSelection(std::vector<std::shared_ptr<NodeId>> idents,
                       std::shared_ptr<NodeType> type)
        : idents(std::move(idents)), type(std::move(type)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    void accept(BaseVisitorWithResult *visitor, bool result) override {
        visitor->visit(this, result);
    }
    std::vector<std::shared_ptr<NodeId>> idents;
    std::shared_ptr<NodeType> type;
};

class NodeRecordType : public NodeType {
   public:
    explicit NodeRecordType(
        std::vector<std::shared_ptr<NodeFieldSelection>> fields)
        : fields(std::move(fields)) {}
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    void accept(BaseVisitorWithResult *visitor, bool result) override {
        visitor->visit(this, result);
    }
    std::vector<std::shared_ptr<NodeFieldSelection>> fields;
};
}  // namespace parser

#endif  // PARSER_NODE_TYPE_H
