#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <any>
#include <iostream>
#include <memory>
#include <vector>

#include "../../scanner/token.h"
#include "../../visitor/base_visitor.h"
#include "../../visitor/printer_visitor.h"
#include "../../visitor/semantic_visitor.h"

namespace parser {
using namespace scanner;
using namespace visitor;

class SyntaxNode {
   public:
    virtual ~SyntaxNode() = default;
    virtual void accept(BaseVisitor *visitor) = 0;
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;
};

class NodeWithStringToken : public SyntaxNode {
   public:
    explicit NodeWithStringToken(Token token) : token(std::move(token)) {}
    virtual std::string get_name() = 0;
    Token get_token();

   protected:
    Token token;
};

class NodeKeyword : public NodeWithStringToken {
   public:
    explicit NodeKeyword(Token token) : NodeWithStringToken(std::move(token)) {}
    std::string get_name() override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;
};
}  // namespace parser

#endif  // PARSER_NODE_H
