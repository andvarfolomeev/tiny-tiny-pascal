#ifndef PARSER_NODE_PROGRAM_H
#define PARSER_NODE_PROGRAM_H

#include "../../scanner/token.h"
#include "node.h"
#include "node_declaration.h"
#include "node_expression.h"
#include "node_statement.h"

namespace parser {
using namespace scanner;

class NodeProgram : SyntaxNode {
   public:
    NodeProgram(std::shared_ptr<NodeId> name, std::shared_ptr<NodeBlock> block)
        : name(std::move(name)), block(std::move(block)) {}
    void draw(std::ostream &os, int depth) override;
    void accept(BaseVisitor *visitor) override { visitor->visit(this); }
    friend class visitor::SemanticVisitor;
    friend class visitor::PrinterVisitor;

   private:
    std::shared_ptr<NodeId> name;
    std::shared_ptr<NodeBlock> block;
};
}  // namespace parser

#endif  // PARSER_NODE_PROGRAM_H
