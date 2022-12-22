#ifndef PARSER_NODE_BASE_EXPRESSION_H
#define PARSER_NODE_BASE_EXPRESSION_H

#include "../../scanner/token.h"
#include "node.h"

namespace parser {
using namespace scanner;

class NodeExpression : public SyntaxNode {};
}  // namespace parser
#endif  // PARSER_NODE_BASE_EXPRESSION_H