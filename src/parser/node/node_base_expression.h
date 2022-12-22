#ifndef PARSER_NODE_BASE_EXPRESSION_H
#define PARSER_NODE_BASE_EXPRESSION_H

#include "../../scanner/token.h"
#include "../../symbol_table/symbol_type.h"
#include "node.h"

namespace parser {
using namespace scanner;

class NodeExpression : public SyntaxNode {
   public:
    virtual std::shared_ptr<SymbolType> calc_sym_type() { return sym_type; }

   protected:
    std::shared_ptr<SymbolType> sym_type = nullptr;
};
}  // namespace parser
#endif  // PARSER_NODE_BASE_EXPRESSION_H