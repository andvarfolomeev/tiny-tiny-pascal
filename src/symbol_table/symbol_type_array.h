#ifndef SYMBOL_TYPE_ARRAY_H
#define SYMBOL_TYPE_ARRAY_H

#include <utility>

#include "../parser/node/node_base_expression.h"
#include "symbol_type.h"

class SymbolArray : public SymbolType {
   public:
    SymbolArray(std::shared_ptr<SymbolType> elem,
                std::vector<std::pair<std::shared_ptr<parser::NodeExpression>,
                                      std::shared_ptr<parser::NodeExpression>>>
                    ranges)
        : SymbolType("array"),
          elem(std::move(std::move(elem))),
          ranges(std::move(ranges)) {}

   protected:
    std::shared_ptr<SymbolType> elem;
    std::vector<std::pair<std::shared_ptr<parser::NodeExpression>,
                          std::shared_ptr<parser::NodeExpression>>>
        ranges;
};

#endif