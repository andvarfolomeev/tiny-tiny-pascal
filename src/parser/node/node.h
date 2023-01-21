#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <any>
#include <iostream>
#include <memory>
#include <vector>

#include "../../scanner/token.h"
#include "../../visitor/base_visitor.h"

namespace parser {
class Parser;
}

namespace parser {
using namespace scanner;
using namespace visitor;

class SyntaxNode {
   public:
    virtual ~SyntaxNode() = default;
    virtual void accept(BaseVisitor *visitor) = 0;
};

class NodeKeyword : public SyntaxNode {
   public:
    explicit NodeKeyword(Token token) : token(std::move(token)) {}
    std::string get_name();
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    Token token;
};
}  // namespace parser

#endif  // PARSER_NODE_H
