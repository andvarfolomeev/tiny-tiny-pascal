#ifndef PARSER_NODE_EXPRESSION_H
#define PARSER_NODE_EXPRESSION_H

#include <utility>

#include "../../scanner/token.h"
#include "node.h"
#include "node_base_expression.h"

namespace parser {
using namespace scanner;

class NodeVarRef : public NodeExpression {};

class NodeId : public NodeVarRef {
   public:
    explicit NodeId(Token token) : token(std::move(token)) {}
    std::string get_name();
    Token &get_token();
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
};

class NodeBoolean : public NodeExpression {
   public:
    explicit NodeBoolean(Token token) : token(std::move(token)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
};

class NodeBinOp : public NodeExpression {
   public:
    NodeBinOp(Token token, std::shared_ptr<NodeExpression> left,
              std::shared_ptr<NodeExpression> right)
        : token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
    std::shared_ptr<NodeExpression> left;
    std::shared_ptr<NodeExpression> right;
};

class NodeUnOp : public NodeExpression {
   public:
    ~NodeUnOp() override = default;
    NodeUnOp(Token token, std::shared_ptr<NodeExpression> operand)
        : token(std::move(token)), operand(std::move(operand)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
    std::shared_ptr<NodeExpression> operand;
};

class NodeRelOp : public NodeExpression {
   public:
    NodeRelOp(Token token, std::shared_ptr<NodeExpression> left,
              std::shared_ptr<NodeExpression> right)
        : token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
    std::shared_ptr<NodeExpression> left;
    std::shared_ptr<NodeExpression> right;
};

class NodeNumber : public NodeExpression {
   public:
    explicit NodeNumber(Token token) : token(std::move(token)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
};

class NodeCast : public NodeExpression {
   public:
    explicit NodeCast(std::shared_ptr<NodeExpression> exp,
                      std::shared_ptr<SymbolType> sym_type)
        : exp(std::move(exp)) {
        this->sym_type = sym_type;
    }
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeExpression> exp;
};

class NodeString : public NodeExpression {
   public:
    explicit NodeString(Token token) : token(std::move(token)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    Token token;
};

class NodeFuncCall : public NodeVarRef {
   public:
    NodeFuncCall(std::shared_ptr<NodeVarRef> var_ref,
                 std::vector<std::shared_ptr<NodeExpression>> params)
        : var_ref(std::move(var_ref)), params(std::move(params)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeVarRef> var_ref;
    std::vector<std::shared_ptr<NodeExpression>> params;
};

class NodeArrayAccess : public NodeVarRef {
   public:
    NodeArrayAccess(std::shared_ptr<NodeVarRef> var_ref,
                    std::shared_ptr<NodeExpression> index)
        : var_ref(std::move(var_ref)), index(std::move(index)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeVarRef> var_ref;
    std::shared_ptr<NodeExpression> index;
};

class NodeRecordAccess : public NodeVarRef {
   public:
    NodeRecordAccess(std::shared_ptr<NodeVarRef> var_ref,
                     std::shared_ptr<NodeId> field)
        : var_ref(std::move(var_ref)), field(std::move(field)) {}
    Position get_pos() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   protected:
    std::shared_ptr<NodeVarRef> var_ref;
    std::shared_ptr<NodeId> field;
};

}  // namespace parser

#endif  // PARSER_NODE_EXPRESSION_H