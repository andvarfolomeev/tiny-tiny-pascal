#ifndef SYMBOL_TYPE_ARRAY_H
#define SYMBOL_TYPE_ARRAY_H

#include <utility>

#include "../parser/node/node_base_expression.h"
#include "symbol_type.h"

class SymbolArray : public SymbolType {
   public:
    SymbolArray(std::shared_ptr<SymbolType> type,
                std::shared_ptr<parser::NodeExpression> low,
                std::shared_ptr<parser::NodeExpression> high)
        : SymbolType("array"),
          type(std::move(std::move(type))),
          low(std::move(low)),
          high(std::move(high)) {}
    std::string get_type_of_object_str() override;
    bool equivalent_to(std::shared_ptr<SymbolType> other) override;
    std::string to_str() override;

   protected:
    std::shared_ptr<SymbolType> type;
    std::shared_ptr<parser::NodeExpression> low;
    std::shared_ptr<parser::NodeExpression> high;
};

#endif