#ifndef PARSER_H
#define PARSER_H

#include <optional>

#include "../scanner/scanner.h"
#include "node.h"

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
    SyntaxNodePointer program();

    SyntaxNodePointer program_name();

    SyntaxNodePointer block();

    SyntaxNodePointer const_decls();
    SyntaxNodePointer const_decl();

    SyntaxNodePointer var_decls();
    SyntaxNodePointer var_decl();

    SyntaxNodePointer type_decls();
    SyntaxNodePointer type_decl();

    SyntaxNodePointer procedure_decl();
    SyntaxNodePointer procedure_header();

    SyntaxNodePointer function_decl();

    SyntaxNodePointer formal_param_section();

    SyntaxNodePointer compound_statement();
    SyntaxNodePointer statement();
    SyntaxNodePointer simple_statement();
    SyntaxNodePointer for_statement();
    SyntaxNodePointer while_statement();
    SyntaxNodePointer do_while_statement();
    SyntaxNodePointer if_statement();
    SyntaxNodePointer assigment_statement();

    SyntaxNodePointer expression();
    SyntaxNodePointer simple_expression();
    SyntaxNodePointer term();
    SyntaxNodePointer simple_term();
    SyntaxNodePointer factor();
    SyntaxNodePointer id_ref(SyntaxNodePointer i);

    SyntaxNodePointers function_call();
    SyntaxNodePointers array_access();
    SyntaxNodePointer record_access();
    SyntaxNodePointers param_list();
    SyntaxNodePointer set_constructor();
    SyntaxNodePointer set_element();
    SyntaxNodePointer identifier();
    SyntaxNodePointer keyword();

    SyntaxNodePointer type();

    SyntaxNodePointer simple_type();

    SyntaxNodePointer array_type();
    SyntaxNodePointers index_ranges();
    SyntaxNodePointer index_range();

    SyntaxNodePointer record_type();
    SyntaxNodePointers fields_list();
    SyntaxNodePointer field_section();

    SyntaxNodePointers list(
        SyntaxNodePointer (Parser::*func)(), Separators sep_type,
        std::optional<Separators> sep_end = {},
        std::optional<Keywords> keyword_end = {});

   private:
    Scanner scanner;
    Token current_token;

    Token next_token();
    SyntaxException new_exception(const std::string &message);

    template <typename T1, typename T2>
    bool check_type(std::shared_ptr<T2> a) {
        return dynamic_cast<T1*>(a.get()) != nullptr;
    }
};
}  // namespace parser

#endif  // PARSER_H
