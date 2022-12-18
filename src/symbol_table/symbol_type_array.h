#ifndef SYMBOL_TYPE_ARRAY_H
#define SYMBOL_TYPE_ARRAY_H

#include "symbol_type.h"
class SymbolArray : public SymbolType {
   public:
    SymbolArray(std::shared_ptr<SymbolType> elem,
                std::vector<std::pair<size_t, size_t>> ranges)
        : SymbolType("array"), elem(elem), ranges(ranges) {}

   protected:
    std::shared_ptr<SymbolType> elem;
    std::vector<std::pair<size_t, size_t>> ranges;
};

#endif