#ifndef SYMBOL_TYPE_ARRAY_H
#define SYMBOL_TYPE_ARRAY_H

#include <utility>

#include "../parser/node/node_base_expression.h"
#include "symbol_type.h"

class SymbolArray : public SymbolType {
   public:
    SymbolArray(std::shared_ptr<SymbolType> type,
                std::vector<std::pair<std::shared_ptr<parser::NodeExpression>,
                                      std::shared_ptr<parser::NodeExpression>>>
                    ranges)
        : SymbolType("array"),
          type(std::move(std::move(type))),
          ranges(std::move(ranges)) {}
    std::string get_type_of_object_str() override;
    bool equivalent_to(std::shared_ptr<SymbolType> other) override;

   protected:
    std::shared_ptr<SymbolType> type;
    std::vector<std::pair<std::shared_ptr<parser::NodeExpression>,
                          std::shared_ptr<parser::NodeExpression>>>
        ranges;
};

#endif