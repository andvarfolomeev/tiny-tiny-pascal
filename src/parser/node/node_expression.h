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
    void draw(std::ostream &os, int depth) override;
    std::string get_name();

   protected:
    Token token;
};

class NodeBoolean : public NodeExpression {
   public:
    explicit NodeBoolean(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeBinOp : public NodeExpression {
   public:
    NodeBinOp(Token token, std::shared_ptr<NodeExpression> left,
              std::shared_ptr<NodeExpression> right)
        : token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
    std::shared_ptr<NodeExpression> left;
    std::shared_ptr<NodeExpression> right;
};

class NodeUnOp : public NodeExpression {
   public:
    ~NodeUnOp() override = default;
    NodeUnOp(Token token, std::shared_ptr<NodeExpression> operand)
        : token(std::move(token)), operand(std::move(operand)) {}
    void draw(std::ostream &os, int depth) override;

   private:
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
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
    std::shared_ptr<NodeExpression> left;
    std::shared_ptr<NodeExpression> right;
};

class NodeNumber : public NodeExpression {
   public:
    explicit NodeNumber(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeString : public NodeExpression {
   public:
    explicit NodeString(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeFuncCall : public NodeVarRef {
   public:
    NodeFuncCall(std::shared_ptr<NodeVarRef> var_ref,
                 std::vector<std::shared_ptr<NodeExpression>> params)
        : var_ref(std::move(var_ref)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

    std::shared_ptr<NodeVarRef> get_var_ref();
    std::vector<std::shared_ptr<NodeExpression>> get_params();

   private:
    std::shared_ptr<NodeVarRef> var_ref;
    std::vector<std::shared_ptr<NodeExpression>> params;
};

class NodeArrayAccess : public NodeVarRef {
   public:
    NodeArrayAccess(std::shared_ptr<NodeVarRef> var_ref,
                    std::vector<std::shared_ptr<NodeExpression>> params)
        : var_ref(std::move(var_ref)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeVarRef> var_ref;
    std::vector<std::shared_ptr<NodeExpression>> params;
};

class NodeRecordAccess : public NodeVarRef {
   public:
    NodeRecordAccess(std::shared_ptr<NodeVarRef> var_ref,
                     std::shared_ptr<NodeId> field)
        : var_ref(std::move(var_ref)), field(std::move(field)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeVarRef> var_ref;
    std::shared_ptr<NodeId> field;
};

class NodeSetElement : public NodeExpression {};

class NodeSetSimpleElement : public NodeSetElement {
   public:
    explicit NodeSetSimpleElement(std::shared_ptr<NodeExpression> exp)
        : exp(std::move(exp)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeExpression> exp;
};

class NodeSetRangeElement : public NodeSetElement {
   public:
    NodeSetRangeElement(std::shared_ptr<NodeExpression> exp1,
                        std::shared_ptr<NodeExpression> exp2)
        : exp1(std::move(exp1)), exp2(std::move(exp2)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeExpression> exp1;
    std::shared_ptr<NodeExpression> exp2;
};

class NodeSetConstructor : public NodeExpression {
   public:
    explicit NodeSetConstructor(
        std::vector<std::shared_ptr<NodeSetElement>> elements)
        : elements(std::move(elements)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::vector<std::shared_ptr<NodeSetElement>> elements;
};

}  // namespace parser

#endif  // PARSER_NODE_EXPRESSION_H
