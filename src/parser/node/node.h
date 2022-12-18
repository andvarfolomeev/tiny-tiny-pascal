#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <iostream>
#include <memory>
#include <vector>

#include "../../scanner/token.h"

namespace parser {
using namespace scanner;

class SyntaxNode {
   public:
    virtual ~SyntaxNode() = default;
    virtual void draw(std::ostream &os, int depth) = 0;
    void draw_path(std::ostream &os, int depth);

    template <typename T>
    void draw_vector(std::ostream &os, int depth,
                     const std::vector<std::shared_ptr<T>> &vec) {
        for (auto &item : vec) {
            os << "\n";
            draw_path(os, depth);
            item->draw(os, depth);
        }
    }
};

class NodeKeyword : public SyntaxNode {
   public:
    NodeKeyword(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;
    std::string get_name();

   protected:
    Token token;
};
}  // namespace parser

#endif  // PARSER_NODE_H
