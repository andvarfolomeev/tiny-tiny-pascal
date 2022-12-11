#ifndef PARSER_NODE_STATEMENT_H
#define PARSER_NODE_STATEMENT_H

#include "../../scanner/token.h"
#include "node.h"
#include "node_expression.h"

namespace parser {
using namespace scanner;

class NodeStatement : public SyntaxNode {};

class NodeCallStatement : public NodeStatement, public NodeFuncCall {
   public:
    NodeCallStatement(std::shared_ptr<NodeVarRef> var_ref,
                      std::vector<std::shared_ptr<NodeExpression>> params)
        : NodeFuncCall(var_ref, params) {}

    NodeCallStatement(std::shared_ptr<NodeFuncCall> func_call)
        : NodeFuncCall(func_call->get_var_ref(), func_call->get_params()) {}

    void draw(std::ostream &os, int depth) override;
};

class NodeCompoundStatement : public NodeStatement {
   public:
    NodeCompoundStatement(std::vector<std::shared_ptr<NodeStatement>> stmts)
        : stmts(std::move(stmts)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::vector<std::shared_ptr<NodeStatement>> stmts;
};

class NodeForStatement : public NodeStatement {
   public:
    NodeForStatement(std::shared_ptr<NodeId> param,
                     std::shared_ptr<NodeExpression> start_exp,
                     std::shared_ptr<NodeKeyword> dir,
                     std::shared_ptr<NodeExpression> end_exp,
                     std::shared_ptr<NodeStatement> stmt)
        : param(std::move(param)),
          start_exp(std::move(start_exp)),
          dir(std::move(dir)),
          end_exp(std::move(end_exp)),
          stmt(std::move(stmt)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeId> param;
    std::shared_ptr<NodeExpression> start_exp;
    std::shared_ptr<NodeKeyword> dir;
    std::shared_ptr<NodeExpression> end_exp;
    std::shared_ptr<NodeStatement> stmt;
};

class NodeWhileStatement : public NodeStatement {
   public:
    NodeWhileStatement(std::shared_ptr<NodeExpression> exp,
                       std::shared_ptr<NodeStatement> stmt)
        : exp(std::move(exp)), stmt(std::move(stmt)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeExpression> exp;
    std::shared_ptr<NodeStatement> stmt;
};

class NodeIfStatement : public NodeStatement {
   public:
    NodeIfStatement(std::shared_ptr<NodeExpression> exp,
                    std::shared_ptr<NodeStatement> stmt,
                    std::shared_ptr<NodeStatement> else_stmt)
        : exp(std::move(exp)),
          stmt(std::move(stmt)),
          else_stmt(std::move(else_stmt)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    std::shared_ptr<NodeExpression> exp;
    std::shared_ptr<NodeStatement> stmt;
    std::shared_ptr<NodeStatement> else_stmt;
};

class NodeAssigmentStatement : public NodeStatement {
   public:
    NodeAssigmentStatement(Token op, std::shared_ptr<NodeVarRef> var_ref,
                           std::shared_ptr<NodeExpression> exp)
        : op(std::move(op)), var_ref(std::move(var_ref)), exp(std::move(exp)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token op;
    std::shared_ptr<NodeVarRef> var_ref;
    std::shared_ptr<NodeExpression> exp;
};
}  // namespace parser

#endif  // PARSER_NODE_STATEMENT_H