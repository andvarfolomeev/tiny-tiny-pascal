#include "node.h"

namespace parser {

std::string NodeKeyword::get_name() {
    auto buffer_in_lower = token.get_raw_value();
    std::transform(buffer_in_lower.begin(), buffer_in_lower.end(),
                   buffer_in_lower.begin(), ::tolower);
    return buffer_in_lower;
}
};  // namespace parser
