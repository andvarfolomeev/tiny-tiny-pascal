#ifndef SIMPLE_PARSER_NODE_H
#define SIMPLE_PARSER_NODE_H

#include <memory>
#include <utility>

#include "../scanner/token.h"

using namespace scanner;

namespace simpleparser {
class SyntaxNode {
   public:
    virtual ~SyntaxNode() = default;
    virtual void draw(std::ostream &os, int depth) = 0;
    virtual std::string to_string() = 0;
};

typedef std::shared_ptr<SyntaxNode> SyntaxNodePointer;

class BinOpNode : public SyntaxNode {
   protected:
    Token token;
    SyntaxNodePointer left;
    SyntaxNodePointer right;

   public:
    ~BinOpNode() override = default;
    BinOpNode(Token token, SyntaxNodePointer left, SyntaxNodePointer right)
        : SyntaxNode(),
          token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    void draw(std::ostream &os, int depth) override;
    std::string to_string() override;
};

class NumberNode : public SyntaxNode {
   protected:
    Token token;

   public:
    ~NumberNode() override = default;
    explicit NumberNode(Token token) : SyntaxNode(), token(std::move(token)) {}
    void draw(std::ostream &os, [[maybe_unused]] int depth) override;
    std::string to_string() override;
};

class IdNode : public SyntaxNode {
   protected:
    Token token;

   public:
    ~IdNode() override = default;
    explicit IdNode(Token token) : SyntaxNode(), token(std::move(token)) {}
    void draw(std::ostream &os, [[maybe_unused]] int depth) override;
    std::string to_string() override;
};
}  // namespace simpleparser

#endif  // SIMPLE_PARSER_NODE_H