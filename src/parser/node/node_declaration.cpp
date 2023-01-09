#include "node_declaration.h"

namespace parser {

std::shared_ptr<NodeCompoundStatement> NodeBlock::get_compound_statement() {
    return compound_statement;
}

};  // namespace parser
