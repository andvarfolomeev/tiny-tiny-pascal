#ifndef SYMBOL_TYPE_ARRAY_H
#define SYMBOL_TYPE_ARRAY_H

#include <utility>

#include "symbol_type.h"

namespace parser {
class NodeExpression;
}

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
    std::shared_ptr<SymbolType> get_inner_type();
    std::pair<std::shared_ptr<parser::NodeExpression>,
              std::shared_ptr<parser::NodeExpression>>
    get_bounds();

   protected:
    std::shared_ptr<SymbolType> type;
    std::shared_ptr<parser::NodeExpression> low;
    std::shared_ptr<parser::NodeExpression> high;
};

#endif