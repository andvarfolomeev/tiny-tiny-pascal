#include "parser.h"
#include "magic_enum.hpp"

namespace parser {
Token Parser::next_token() {
    current_token = scanner.next_token();
    return current_token;
}

SyntaxException Parser::new_exception(const std::string& message) {
    auto pos = current_token.get_pos();
    return SyntaxException(pos.first, pos.second, message);
}

SyntaxNodePointer Parser::program() {
    // first token taken in constructor
    SyntaxNodePointer name = nullptr;
    if (current_token == Keywords::PROGRAM) {
        next_token();
        auto name_tmp = program_name();
        name.swap(name_tmp);
    }
    auto block_ = block();
    if (current_token != Separators::PERIOD) {
        throw new_exception(". expected");
    }
    return std::make_shared<NodeProgram>(name, block_);
}

SyntaxNodePointer Parser::program_name() {
    auto name = identifier();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();
    return name;
}

SyntaxNodePointer Parser::block() {
    auto declarations = std::make_shared<std::vector<SyntaxNodePointer>>();
    for (;;) {
        if (current_token == Keywords::CONST) {
            next_token();
            declarations->push_back(const_decls());
        } else if (current_token == Keywords::VAR) {
            next_token();
            declarations->push_back(var_decls());
        } else if (current_token == Keywords::TYPE) {
            next_token();
            declarations->push_back(type_decls());
        } else if (current_token == Keywords::PROCEDURE) {
            next_token();
            declarations->push_back(procedure_decl());
        } else if (current_token == Keywords::FUNCTION) {
            next_token();
            declarations->push_back(function_decl());
        } else {
            break;
        }
    }
    if (current_token != Keywords::BEGIN) {
        throw new_exception("begin expected");
    }
    next_token();
    auto statements = compound_statement();
    return std::make_shared<NodeProgramBlock>(declarations, statements);
}

SyntaxNodePointer Parser::const_decls() {
    auto consts = std::make_shared<std::vector<SyntaxNodePointer>>();
    while (current_token == TokenType::ID) {
        consts->push_back(const_decl());
    }
    return std::make_shared<NodeConstDecls>(consts);
}

SyntaxNodePointer Parser::const_decl() {
    auto id = identifier();
    SyntaxNodePointer id_type = nullptr;
    if (current_token == Separators::COLON) {
        next_token();
        auto id_type_tmp = type();
        id_type.swap(id_type_tmp);
    }
    if (current_token != Operators::EQL) {
        throw new_exception("= expected");
    }
    next_token();
    auto exp = expression();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();

    return std::make_shared<NodeConstDecl>(id, id_type, exp);
}

SyntaxNodePointer Parser::var_decls() {
    auto vars = std::make_shared<std::vector<SyntaxNodePointer>>();
    while (current_token == TokenType::ID) {
        vars->push_back(var_decl());
    }
    return std::make_shared<NodeVarDecls>(vars);
}

SyntaxNodePointer Parser::var_decl() {
    auto ids = list(&Parser::identifier, Separators::COMMA, {}, {});
    if (current_token != Separators::COLON) {
        throw new_exception(": expected");
    }
    next_token();
    auto ids_type = type();
    SyntaxNodePointer exp = nullptr;
    if (current_token == Operators::EQL) {
        if (ids->size() != 1) {
            throw new_exception("Only one variable can be initialized");
        }
        next_token();
        auto exp_tmp = expression();
        exp.swap(exp_tmp);
    }
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();
    return std::make_shared<NodeVarDecl>(ids, ids_type, exp);
}

SyntaxNodePointer Parser::type_decls() {
    auto types = std::make_shared<std::vector<SyntaxNodePointer>>();
    while (current_token == TokenType::ID) {
        types->push_back(type_decl());
    }
    return std::make_shared<NodeTypeDecls>(types);
};

SyntaxNodePointer Parser::type_decl() {
    auto id = identifier();
    if (current_token != Operators::EQL) {
        throw new_exception("= expected");
    }
    next_token();
    auto id_type = type();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();
    return std::make_shared<NodeTypeDecl>(id, id_type);
}

SyntaxNodePointer Parser::procedure_decl() {
    auto id = identifier();
    if (current_token != Separators::LPAREN) {
        throw new_exception("( expected");
    }
    next_token();
    auto params = list(&Parser::formal_param_section, Separators::SEMICOLON,
                       Separators::RPAREN, {});
    if (current_token != Separators::RPAREN) {
        throw new_exception(") expected");
    }
    next_token();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();
    auto block_ = block();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception(
            "; expected in procedure declarations after compound statement");
    }
    next_token();
    return std::make_shared<NodeProcedureDecl>(
        std::make_shared<NodeHeaderProcedureDecl>(id, params), block_);
}

SyntaxNodePointer Parser::function_decl() {
    auto id = identifier();
    if (current_token != Separators::LPAREN) {
        throw new_exception("( expected");
    }
    next_token();
    auto params = list(&Parser::formal_param_section, Separators::SEMICOLON,
                       Separators::RPAREN, {});
    if (current_token != Separators::RPAREN) {
        throw new_exception(") expected");
    }
    next_token();
    if (current_token != Separators::COLON) {
        throw new_exception(": expected");
    }
    next_token();
    auto function_type = type();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception("; expected");
    }
    next_token();
    auto block_ = block();
    if (current_token != Separators::SEMICOLON) {
        throw new_exception(
            "; expected in function declarations after compound statement");
    }
    next_token();
    return std::make_shared<NodeFunctionDecl>(
        std::make_shared<NodeHeaderFunctionDecl>(id, params, function_type),
        block_);
}

SyntaxNodePointer Parser::formal_param_section() {
    SyntaxNodePointer modifier = nullptr;
    if (current_token == Keywords::VAR) {
        modifier = keyword();
    } else if (current_token == Keywords::CONST) {
        modifier = keyword();
    }
    auto idents = list(&Parser::identifier, Separators::COMMA, {}, {});
    if (current_token != Separators::COLON) {
        throw new_exception(": expected");
    }
    next_token();
    return std::make_shared<NodeFormalParamSection>(modifier, idents, type());
}

SyntaxNodePointer Parser::compound_statement() {
    auto stmts = std::make_shared<std::vector<SyntaxNodePointer>>();
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
        if (!stmts->empty() && i == 0) {
            throw new_exception("; expected in compound statement");
        }
        stmts->push_back(statement());
    }
    // TODO: improve exception
    return std::make_shared<NodeCompoundStatement>(stmts);
}

SyntaxNodePointer Parser::statement() {
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

SyntaxNodePointer Parser::simple_statement() {
    auto left = expression();
    if (check_type<NodeFuncCall>(left)) {
        return left;
    } else if (check_type<NodeVarRef>(left)) { 
        if (!current_token.is({Operators::ASSIGN, Operators::ADDASSIGN,
                               Operators::SUBASSIGN, Operators::MULASSIGN,
                               Operators::QUOASSIGN})) {
            return std::make_shared<NodeFuncCall>(left, nullptr);
        }
        auto op = current_token;
        next_token();
        return std::make_shared<NodeAssigmentStatement>(op, left, expression());
    }
    throw new_exception("Illegal statement");
}

SyntaxNodePointer Parser::expression() {
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

SyntaxNodePointer Parser::simple_expression() {
    auto left = term();
    while (current_token.is({Operators::ADD, Operators::SUB}) ||
           current_token.is({Keywords::OR, Keywords::XOR})) {
        auto op = current_token;
        next_token();
        left = std::make_shared<NodeBinOp>(op, left, term());
    }
    return left;
}

SyntaxNodePointer Parser::for_statement() {
    auto id = identifier();
    if (current_token != Operators::ASSIGN) {
        throw new_exception(":= expected");
    }
    next_token();
    auto start_exp = expression();
    if (!current_token.is({Keywords::TO, Keywords::DOWNTO})) {
        throw new_exception("to or downto expected");
    }
    auto dir = keyword();
    auto end_exp = expression();
    if (current_token != Keywords::DO) {
        throw new_exception("do expected");
    }
    next_token();
    auto op = statement();
    return std::make_shared<NodeForStatement>(id, start_exp, dir, end_exp, op);
}

SyntaxNodePointer Parser::while_statement() {
    auto bool_exp = expression();
    if (current_token != Keywords::DO) {
        throw new_exception("do expected in while statement");
    }
    next_token();
    return std::make_shared<NodeWhileStatement>(bool_exp, statement());
}

SyntaxNodePointer Parser::if_statement() {
    auto exp = expression();
    if (current_token != Keywords::THEN) {
        throw new_exception("then expected");
    }
    next_token();
    auto op = statement();
    if (current_token == Keywords::ELSE) {
        next_token();
        auto else_op = statement();

        return std::make_shared<NodeIfStatement>(exp, op, else_op);
    }
    return std::make_shared<NodeIfStatement>(exp, op, nullptr);
}

SyntaxNodePointer Parser::term() {
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

SyntaxNodePointer Parser::simple_term() {
    if (current_token.is({Operators::ADD, Operators::SUB}) ||
        current_token == Keywords::NOT) {
        auto un_op = current_token;
        next_token();
        return std::make_shared<NodeUnOp>(un_op, simple_term());
    }
    return factor();
}

SyntaxNodePointer Parser::factor() {
    auto token = current_token;
    if (token == Keywords::NIL) {
        next_token();
        return std::make_shared<NodeNil>(token);
    }
    if (token == TokenType::LITERAL_STRING) {
        next_token();
        return std::make_shared<NodeString>(token);
    }
    if (token.is({TokenType::LITERAL_DOUBLE, TokenType::LITERAL_INTEGER})) {
        next_token();
        return std::make_shared<NodeNumber>(token);
    }
    if (token == TokenType::ID) {
        return id_ref(identifier());
    }
    if (token == Separators::LPAREN) {
        next_token();
        auto exp = expression();
        if (current_token != Separators::RPAREN) {
            throw new_exception(") expected");
        }
        next_token();
        return exp;
    }
    if (token == Separators::LBRACK) {
        next_token();
        auto set_constr = set_constructor();
        if (current_token != Separators::RBRACK) {
            throw new_exception("] expected");
        }
        next_token();
        return set_constr;
    }
    throw new_exception("factor expected");
}

SyntaxNodePointer Parser::id_ref(SyntaxNodePointer i) {
    auto left = i;
    while (true) {
        if (current_token == Separators::LPAREN) {
            next_token();
            left = std::make_shared<NodeFuncCall>(left, function_call());
        } else if (current_token == Separators::LBRACK) {
            next_token();
            left = std::make_shared<NodeArrayAccess>(left, array_access());
        } else if (current_token == Separators::PERIOD) {
            next_token();
            left = std::make_shared<NodeRecordAccess>(left, record_access());
        } else {
            break;
        }
    }
    return left;
}

SyntaxNodePointers Parser::function_call() {
    auto params = param_list();
    if (current_token != Separators::RPAREN) {
        throw new_exception(") expected");
    }
    next_token();
    return params;
}

SyntaxNodePointers Parser::array_access() {
    auto params = param_list();
    if (current_token != Separators::RBRACK) {
        throw new_exception("] expected");
    }
    next_token();
    return params;
}

SyntaxNodePointer Parser::record_access() { return identifier(); }

SyntaxNodePointers Parser::param_list() {
    return list(&Parser::expression, Separators::COMMA, Separators::RPAREN, {});
}

SyntaxNodePointer Parser::set_constructor() {
    auto elements = std::make_shared<std::vector<SyntaxNodePointer>>();
    elements->push_back(set_element());
    while (current_token == Separators::COMMA) {
        next_token();
        elements->push_back(set_element());
    }
    return std::make_shared<NodeSetConstructor>(elements);
}

SyntaxNodePointer Parser::set_element() {
    auto exp = expression();
    if (current_token == Separators::ELLIPSIS) {
        next_token();
        return std::make_shared<NodeSetRangeElement>(exp, expression());
    }
    return std::make_shared<NodeSetSimpleElement>(exp);
}

SyntaxNodePointer Parser::identifier() {
    if (current_token != TokenType::ID) {
        throw new_exception("id expected");
    }
    auto res = std::make_shared<NodeId>(current_token);
    next_token();
    return res;
}

SyntaxNodePointer Parser::keyword() {
    if (current_token != TokenType::KEYWORD) {
        throw new_exception("keyword expected");
    }
    auto res = std::make_shared<NodeId>(current_token);
    next_token();
    return res;
}

SyntaxNodePointer Parser::type() {
    if (current_token == Keywords::ARRAY) {
        next_token();
        return array_type();
    } else if (current_token == Keywords::RECORD) {
        next_token();
        return record_type();
    } else if (current_token == TokenType::ID || current_token == Keywords::STRING) {
        return simple_type();
    }
    throw new_exception("type expected");
}

SyntaxNodePointer Parser::simple_type() {
    if (current_token == Keywords::STRING) {
        return std::make_shared<NodeSimpleType>(keyword());
    }
    return std::make_shared<NodeSimpleType>(identifier());
}

SyntaxNodePointer Parser::array_type() {
    if (current_token != Separators::LBRACK) {
        throw new_exception("[ expected in array declarations");
    }
    next_token();
    auto ranges = index_ranges();
    if (current_token != Separators::RBRACK) {
        throw new_exception("] expected in array declarations");
    }
    if (next_token() != Keywords::OF) {
        throw new_exception("of expected in array declarations");
    }
    next_token();
    return std::make_shared<NodeArrayType>(type(), ranges);
}

SyntaxNodePointers Parser::index_ranges() {
    return list(&Parser::index_range, Separators::COMMA, {}, {});
}

SyntaxNodePointer Parser::index_range() {
    auto exp1 = expression();
    if (current_token != Separators::ELLIPSIS) {
        throw new_exception(".. expected");
    }
    next_token();
    auto exp2 = expression();
    return std::make_shared<NodeArrayIndexRange>(exp1, exp2);
}

SyntaxNodePointer Parser::record_type() {
    auto fields = fields_list();
    if (current_token == Separators::SEMICOLON) {
        next_token();
    }
    if (next_token() == Keywords::END) {
        throw new_exception("end expected");
    }
    return std::make_shared<NodeRecordType>(fields);
}

SyntaxNodePointers Parser::fields_list() {
    auto fields =
        list(&Parser::field_section, Separators::SEMICOLON, {}, Keywords::END);
    return fields;
}

SyntaxNodePointer Parser::field_section() {
    auto idents =
        list(&Parser::identifier, Separators::COMMA, Separators::COLON, {});
    if (current_token != Separators::COLON) {
        throw new_exception("colon expected");
    }
    next_token();
    return std::make_shared<NodeFieldSelection>(idents, type());
}

SyntaxNodePointers Parser::list(SyntaxNodePointer (Parser::*func)(),
                                Separators sep_type,
                                std::optional<Separators> sep_end,
                                std::optional<Keywords> keyword_end) {
    auto result = std::make_shared<std::vector<SyntaxNodePointer>>();
    if (sep_end.has_value() && current_token == sep_end.value()) {
        return result;
    }
    if (keyword_end.has_value() && current_token == keyword_end.value()) {
        return result;
    }
    result->push_back((this->*func)());
    while (current_token == sep_type) {
        next_token();
        if (sep_end.has_value() && current_token == sep_end.value()) {
            std::stringstream msg;
            msg << magic_enum::enum_name(sep_end.value());
            msg << " found, but element expected";
            throw new_exception(msg.str());
            break;
        }
        if (keyword_end.has_value() && current_token == keyword_end.value()) {
            break;
        }
        result->push_back((this->*func)());
    }
    return result;
}
};  // namespace parser
