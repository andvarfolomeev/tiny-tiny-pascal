#include "parser.h"

#include <utility>

#include "magic_enum.hpp"

template <typename InputIt>
std::string join_enum(InputIt first, InputIt last,
                      const std::string& separator = ", ",
                      const std::string& basic_res = "") {
    if (first == last) {
        return basic_res;
    }

    std::stringstream ss;
    ss << *first;
    ++first;

    while (first != last) {
        ss << separator;
        ss << magic_enum::enum_name(*first);
        ++first;
    }

    return ss.str();
}

namespace parser {
std::shared_ptr<NodeProgram> Parser::program() {
    // first token taken in constructor
    std::shared_ptr<NodeId> name = nullptr;
    if (current_token == Keywords::PROGRAM) {
        next_token();
        auto name_tmp = program_name();
        name.swap(name_tmp);
    }
    auto block_ = block();
    require(Separators::PERIOD);
    return std::make_shared<NodeProgram>(name, block_);
}

std::shared_ptr<NodeId> Parser::program_name() {
    auto name = identifier();
    require(Separators::SEMICOLON);
    return name;
}

std::shared_ptr<NodeBlock> Parser::block() {
    auto declarations = std::vector<std::shared_ptr<NodeDecl>>();
    for (;;) {
        if (current_token == Keywords::CONST) {
            next_token();
            declarations.push_back(const_decls());
        } else if (current_token == Keywords::VAR) {
            next_token();
            declarations.push_back(var_decls());
        } else if (current_token == Keywords::TYPE) {
            next_token();
            declarations.push_back(type_decls());
        } else if (current_token == Keywords::PROCEDURE) {
            next_token();
            declarations.push_back(procedure_decl());
        } else if (current_token == Keywords::FUNCTION) {
            next_token();
            declarations.push_back(function_decl());
        } else {
            break;
        }
    }
    require(Keywords::BEGIN);
    auto statements = compound_statement();
    return std::make_shared<NodeBlock>(declarations, statements);
}

std::shared_ptr<NodeConstDecls> Parser::const_decls() {
    auto consts = std::vector<std::shared_ptr<NodeConstDecl>>();
    while (current_token == TokenType::ID) {
        consts.push_back(const_decl());
    }
    return std::make_shared<NodeConstDecls>(consts);
}

std::shared_ptr<NodeConstDecl> Parser::const_decl() {
    auto id = identifier();
    std::shared_ptr<NodeType> id_type = nullptr;
    if (current_token == Separators::COLON) {
        next_token();
        auto id_type_tmp = type();
        id_type.swap(id_type_tmp);
    }
    require(Operators::EQL);
    auto exp = expression();
    require(Separators::SEMICOLON);
    return std::make_shared<NodeConstDecl>(id, id_type, exp);
}

std::shared_ptr<NodeVarDecls> Parser::var_decls() {
    auto vars = std::vector<std::shared_ptr<NodeVarDecl>>();
    while (current_token == TokenType::ID) {
        vars.push_back(var_decl());
    }
    return std::make_shared<NodeVarDecls>(vars);
}

std::shared_ptr<NodeVarDecl> Parser::var_decl() {
    auto ids = list(&Parser::identifier, Separators::COMMA, {}, {});
    require(Separators::COLON);
    auto ids_type = type();
    std::shared_ptr<NodeExpression> exp = nullptr;
    if (current_token == Operators::EQL) {
        if (ids.size() != 1) {
            throw new_exception("Only one variable can be initialized");
        }
        next_token();
        auto exp_tmp = expression();
        exp.swap(exp_tmp);
    }
    require(Separators::SEMICOLON);
    return std::make_shared<NodeVarDecl>(ids, ids_type, exp);
}

std::shared_ptr<NodeTypeDecls> Parser::type_decls() {
    auto types = std::vector<std::shared_ptr<NodeTypeDecl>>();
    while (current_token == TokenType::ID) {
        types.push_back(type_decl());
    }
    return std::make_shared<NodeTypeDecls>(types);
};

std::shared_ptr<NodeTypeDecl> Parser::type_decl() {
    auto id = identifier();
    require(Operators::EQL);
    auto id_type = type();
    require(Separators::SEMICOLON);
    return std::make_shared<NodeTypeDecl>(id, id_type);
}

std::shared_ptr<NodeProcedureDecl> Parser::procedure_decl() {
    auto id = identifier();
    require(Separators::LPAREN);
    auto params = list(&Parser::formal_param_section, Separators::SEMICOLON,
                       Separators::RPAREN, {});
    require(Separators::RPAREN);
    require(Separators::SEMICOLON);
    auto block_ = block();
    require(Separators::SEMICOLON);
    return std::make_shared<NodeProcedureDecl>(id, params, block_);
}

std::shared_ptr<NodeFunctionDecl> Parser::function_decl() {
    auto id = identifier();
    require(Separators::LPAREN);
    auto params = list(&Parser::formal_param_section, Separators::SEMICOLON,
                       Separators::RPAREN, {});
    require(Separators::RPAREN);
    require(Separators::COLON);
    auto function_type = type();
    require(Separators::SEMICOLON);
    auto block_ = block();
    require(Separators::SEMICOLON);
    return std::make_shared<NodeFunctionDecl>(id, params, block_,
                                              function_type);
}

std::shared_ptr<NodeFormalParamSection> Parser::formal_param_section() {
    std::shared_ptr<NodeKeyword> modifier = nullptr;
    if (current_token == Keywords::VAR) {
        modifier = keyword();
    } else if (current_token == Keywords::CONST) {
        modifier = keyword();
    }
    auto idents = list(&Parser::identifier, Separators::COMMA, {}, {});
    require(Separators::COLON);
    return std::make_shared<NodeFormalParamSection>(modifier, idents, type());
}

std::shared_ptr<NodeCompoundStatement> Parser::compound_statement() {
    auto stmts = std::vector<std::shared_ptr<NodeStatement>>();
    while (true) {
        int i = 0;
        while (current_token == Separators::SEMICOLON) {
            i++;
            next_token();
        }
        if (current_token == Keywords::END) {
            next_token();
            break;
        }
        if (!stmts.empty() && i == 0) {
            require(Separators::SEMICOLON);
        }
        stmts.push_back(statement());
    }
    // TODO: improve exception
    return std::make_shared<NodeCompoundStatement>(stmts);
}

std::shared_ptr<NodeStatement> Parser::statement() {
    if (current_token == Keywords::IF) {
        next_token();
        return if_statement();
    } else if (current_token == Keywords::WHILE) {
        next_token();
        return while_statement();
    } else if (current_token == Keywords::FOR) {
        next_token();
        return for_statement();
    } else if (current_token == Keywords::BEGIN) {
        next_token();
        return compound_statement();
    }
    return simple_statement();
}

std::shared_ptr<NodeStatement> Parser::simple_statement() {
    auto left = expression();
    if (check_type<NodeFuncCall>(left)) {
        auto func_call = std::dynamic_pointer_cast<NodeFuncCall>(left);
        return std::make_shared<NodeCallStatement>(func_call);
    } else if (check_type<NodeVarRef>(left)) {
        auto var_ref = std::dynamic_pointer_cast<NodeVarRef>(left);
        if (!current_token.is({Operators::ASSIGN, Operators::ADDASSIGN,
                               Operators::SUBASSIGN, Operators::MULASSIGN,
                               Operators::QUOASSIGN})) {
            return std::make_shared<NodeCallStatement>(
                var_ref, std::vector<std::shared_ptr<NodeExpression>>());
        }
        auto op = current_token;
        next_token();
        return std::make_shared<NodeAssigmentStatement>(op, var_ref,
                                                        expression());
    }
    throw new_exception("Illegal statement");
}

std::shared_ptr<NodeForStatement> Parser::for_statement() {
    auto id = identifier();
    require(Operators::ASSIGN);
    auto start_exp = expression();
    require(Keywords::TO, Keywords::DOWNTO, false);
    auto dir = keyword();
    auto end_exp = expression();
    require(Keywords::DO);
    auto op = statement();
    return std::make_shared<NodeForStatement>(id, start_exp, dir, end_exp, op);
}

std::shared_ptr<NodeWhileStatement> Parser::while_statement() {
    auto bool_exp = expression();
    require(Keywords::DO);
    return std::make_shared<NodeWhileStatement>(bool_exp, statement());
}

std::shared_ptr<NodeIfStatement> Parser::if_statement() {
    auto exp = expression();
    require(Keywords::THEN);
    auto op = statement();
    if (current_token == Keywords::ELSE) {
        next_token();
        auto else_op = statement();

        return std::make_shared<NodeIfStatement>(exp, op, else_op);
    }
    return std::make_shared<NodeIfStatement>(exp, op, nullptr);
}

std::shared_ptr<NodeExpression> Parser::expression() {
    auto left = simple_expression();
    while (current_token.is({Operators::EQL, Operators::LES, Operators::NEQ,
                             Operators::LEQ, Operators::GTR, Operators::GEQ}) ||
           current_token.is({Keywords::OR, Keywords::IN})) {
        auto op = current_token;
        next_token();
        left = std::make_shared<NodeRelOp>(op, left, simple_expression());
    }

    return left;
}

std::shared_ptr<NodeExpression> Parser::simple_expression() {
    auto left = term();
    while (current_token.is({Operators::ADD, Operators::SUB}) ||
           current_token.is({Keywords::OR, Keywords::XOR})) {
        auto op = current_token;
        next_token();
        left = std::make_shared<NodeBinOp>(op, left, term());
    }
    return left;
}

std::shared_ptr<NodeExpression> Parser::term() {
    auto left = simple_term();
    while (current_token.is({Operators::MUL, Operators::QUO}) ||
           current_token.is({Keywords::DIV, Keywords::MOD, Keywords::AND,
                             Keywords::SHR, Keywords::SHL})) {
        auto op = current_token;
        next_token();
        left = std::make_shared<NodeBinOp>(op, left, simple_term());
    }
    return left;
}

std::shared_ptr<NodeExpression> Parser::simple_term() {
    if (current_token.is({Operators::ADD, Operators::SUB}) ||
        current_token == Keywords::NOT) {
        auto un_op = current_token;
        next_token();
        return std::make_shared<NodeUnOp>(un_op, simple_term());
    }
    return factor();
}

std::shared_ptr<NodeExpression> Parser::factor() {
    auto token = current_token;
    if (token == TokenType::LITERAL_STRING) {
        next_token();
        return std::make_shared<NodeString>(token);
    }
    if (token.is({TokenType::LITERAL_DOUBLE, TokenType::LITERAL_INTEGER})) {
        next_token();
        return std::make_shared<NodeNumber>(token);
    }
    if (token == TokenType::ID) {
        return var_ref(identifier());
    }
    if (token == Separators::LPAREN) {
        next_token();
        auto exp = expression();
        require(Separators::RPAREN);
        return exp;
    }
    if (token == Separators::LBRACK) {
        next_token();
        auto set_constr = set_constructor();
        require(Separators::RBRACK);
        return set_constr;
    }
    throw new_exception("factor expected");
}

std::shared_ptr<NodeVarRef> Parser::var_ref(std::shared_ptr<NodeVarRef> i) {
    auto left = std::move(i);
    while (true) {
        if (current_token == Separators::LPAREN) {
            next_token();
            auto params = list(&Parser::expression, Separators::COMMA,
                               Separators::RPAREN, {});
            require(Separators::RPAREN);
            left = std::make_shared<NodeFuncCall>(left, params);
        } else if (current_token == Separators::LBRACK) {
            next_token();
            auto params = list(&Parser::expression, Separators::COMMA,
                               Separators::RBRACK, {});
            require(Separators::RBRACK);
            left = std::make_shared<NodeArrayAccess>(left, params);
        } else if (current_token == Separators::PERIOD) {
            next_token();
            left = std::make_shared<NodeRecordAccess>(left, identifier());
        } else {
            break;
        }
    }
    return left;
}

std::shared_ptr<NodeSetConstructor> Parser::set_constructor() {
    auto elements = std::vector<std::shared_ptr<NodeSetElement>>();
    elements.push_back(set_element());
    while (current_token == Separators::COMMA) {
        next_token();
        elements.push_back(set_element());
    }
    return std::make_shared<NodeSetConstructor>(elements);
}

std::shared_ptr<NodeSetElement> Parser::set_element() {
    auto exp = expression();
    if (current_token == Separators::ELLIPSIS) {
        next_token();
        return std::make_shared<NodeSetRangeElement>(exp, expression());
    }
    return std::make_shared<NodeSetSimpleElement>(exp);
}

std::shared_ptr<NodeId> Parser::identifier() {
    require(TokenType::ID, false);
    auto res = std::make_shared<NodeId>(current_token);
    next_token();
    return res;
}

std::shared_ptr<NodeKeyword> Parser::keyword() {
    require(TokenType::KEYWORD, false);
    auto res = std::make_shared<NodeKeyword>(current_token);
    next_token();
    return res;
}

std::shared_ptr<NodeType> Parser::type() {
    if (current_token == Keywords::ARRAY) {
        next_token();
        return array_type();
    } else if (current_token == Keywords::RECORD) {
        next_token();
        return record_type();
    } else if (current_token == TokenType::ID ||
               current_token == Keywords::STRING) {
        return simple_type();
    }
    throw new_exception("type expected");
}

std::shared_ptr<NodeType> Parser::simple_type() {
    if (current_token == Keywords::STRING) {
        return std::make_shared<NodeSimpleType>(keyword());
    }
    return std::make_shared<NodeSimpleType>(identifier());
}

std::shared_ptr<NodeArrayType> Parser::array_type() {
    require(Separators::LBRACK);
    auto ranges = list(&Parser::index_range, Separators::COMMA, {}, {});
    require(Separators::RBRACK);
    require(Keywords::OF);
    return std::make_shared<NodeArrayType>(type(), ranges);
}

std::shared_ptr<NodeRange> Parser::index_range() {
    auto exp1 = expression();
    require(Separators::ELLIPSIS);
    auto exp2 = expression();
    return std::make_shared<NodeRange>(exp1, exp2);
}

std::shared_ptr<NodeRecordType> Parser::record_type() {
    auto fields =
        list(&Parser::field_section, Separators::SEMICOLON, {}, Keywords::END);
    if (current_token == Separators::SEMICOLON) {
        next_token();
    }
    require(Keywords::END);
    return std::make_shared<NodeRecordType>(fields);
}

std::shared_ptr<NodeFieldSelection> Parser::field_section() {
    auto idents =
        list(&Parser::identifier, Separators::COMMA, Separators::COLON, {});
    require(Separators::COLON);
    return std::make_shared<NodeFieldSelection>(idents, type());
}

template <typename T>
std::vector<T> Parser::list(T (Parser::*func)(), Separators sep_type,
                            std::optional<Separators> sep_end,
                            std::optional<Keywords> keyword_end) {
    auto result = std::vector<T>();
    if (sep_end.has_value() && current_token == sep_end.value()) {
        return result;
    }
    if (keyword_end.has_value() && current_token == keyword_end.value()) {
        return result;
    }
    result.push_back((this->*func)());
    while (current_token == sep_type) {
        next_token();
        if (sep_end.has_value() && current_token == sep_end.value()) {
            std::stringstream msg;
            msg << magic_enum::enum_name(sep_end.value());
            msg << " found, but element expected";
            throw new_exception(msg.str());
        }
        if (keyword_end.has_value() && current_token == keyword_end.value()) {
            break;
        }
        result.push_back((this->*func)());
    }
    return result;
}

Token Parser::next_token() {
    current_token = scanner.next_token();
    return current_token;
}

template <typename T>
void Parser::require_vec(std::vector<T> items, bool eat) {
    if (!current_token.is(items)) {
        std::stringstream msg;
        msg << "Expected: ";
        msg << join_enum(items.begin(), items.end(), "or");
        msg << "; but found " << current_token.get_raw_value();
        throw new_exception(msg.str());
    }
    if (eat) {
        next_token();
    }
}

void Parser::require(Keywords keyword, bool eat) {
    return require_vec<Keywords>({keyword}, eat);
}

void Parser::require(Keywords keyword1, Keywords keyword2, bool eat) {
    return require_vec<Keywords>({keyword1, keyword2}, eat);
}

void Parser::require(Separators sep, bool eat) {
    return require_vec<Separators>({sep}, eat);
}

void Parser::require(Operators op, bool eat) {
    return require_vec<Operators>({op}, eat);
}

void Parser::require(TokenType type, bool eat) {
    return require_vec<TokenType>({type}, eat);
}

SyntaxException Parser::new_exception(const std::string& message) {
    auto pos = current_token.get_pos();
    return SyntaxException(pos.first, pos.second, message);
}

template <typename T1, typename T2>
bool Parser::check_type(std::shared_ptr<T2> a) {
    return dynamic_cast<T1*>(a.get()) != nullptr;
}
};  // namespace parser
