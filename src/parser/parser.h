#ifndef PARSER_H
#define PARSER_H

#include <optional>

#include "../scanner/scanner.h"
#include "node/nodes.h"

using namespace scanner;

namespace parser {
class SyntaxException : public TinyPascalException {
   protected:
    std::string message;

   public:
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }

    [[maybe_unused]] explicit SyntaxException(unsigned int current_line,
                                              unsigned int current_column,
                                              const std::string &message) {
        std::ostringstream string_stream;
        string_stream << "Line: " << current_line
                      << "; Column: " << current_column << "; " << message;
        this->message = string_stream.str();
    }
};

class Parser {
   public:
    explicit Parser(Scanner &scanner)
        : scanner(scanner), current_token(scanner.next_token()) {}
    std::shared_ptr<NodeProgram> program();

    std::shared_ptr<NodeId> program_name();

    std::shared_ptr<NodeBlock> block();

    std::shared_ptr<NodeConstDecls> const_decls();
    std::shared_ptr<NodeConstDecl> const_decl();

    std::shared_ptr<NodeVarDecls> var_decls();
    std::shared_ptr<NodeVarDecl> var_decl();

    std::shared_ptr<NodeTypeDecls> type_decls();
    std::shared_ptr<NodeTypeDecl> type_decl();

    std::shared_ptr<NodeProcedureDecl> procedure_decl();
    std::shared_ptr<NodeFunctionDecl> function_decl();
    std::shared_ptr<NodeFormalParamSection> formal_param_section();

    std::shared_ptr<NodeCompoundStatement> compound_statement();
    std::shared_ptr<NodeStatement> statement();
    std::shared_ptr<NodeStatement> simple_statement();
    std::shared_ptr<NodeForStatement> for_statement();
    std::shared_ptr<NodeWhileStatement> while_statement();
    std::shared_ptr<NodeIfStatement> if_statement();

    std::shared_ptr<NodeExpression> expression();
    std::shared_ptr<NodeExpression> simple_expression();
    std::shared_ptr<NodeExpression> term();
    std::shared_ptr<NodeExpression> simple_term();
    std::shared_ptr<NodeExpression> factor();
    std::shared_ptr<NodeVarRef> var_ref(std::shared_ptr<NodeVarRef> i);

    std::shared_ptr<NodeSetConstructor> set_constructor();
    std::shared_ptr<NodeSetElement> set_element();
    std::shared_ptr<NodeId> identifier();
    std::shared_ptr<NodeKeyword> keyword();

    std::shared_ptr<NodeType> type();

    std::shared_ptr<NodeType> simple_type();

    std::shared_ptr<NodeArrayType> array_type();
    std::shared_ptr<NodeRange> index_range();

    std::shared_ptr<NodeRecordType> record_type();
    std::shared_ptr<NodeFieldSelection> field_section();

    template <typename T>
    std::vector<T> list(T (Parser::*func)(), Separators sep_type,
                        std::optional<Separators> sep_end = {},
                        std::optional<Keywords> keyword_end = {});

   private:
    Scanner scanner;
    Token current_token;

    Token next_token();

    template <typename T>
    void require_vec(const std::vector<T> &items, bool eat = true);
    void require(Keywords keyword, bool eat = true);
    void require(Keywords keyword1, Keywords keyword2, bool eat = true);
    void require(Separators sep, bool eat = true);
    void require(Operators op, bool eat = true);
    void require(TokenType type, bool eat = true);

    SyntaxException new_exception(const std::string &message);
    template <typename T1, typename T2>

    bool check_type(std::shared_ptr<T2> a);
};
}  // namespace parser

#endif  // PARSER_H
