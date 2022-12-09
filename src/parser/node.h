#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <memory>
#include <utility>
#include <vector>

#include "../scanner/token.h"

using namespace scanner;

namespace parser {
class SyntaxNode {
   public:
    virtual ~SyntaxNode() = default;
    virtual void draw(std::ostream &os, int depth) = 0;
    void draw_path(std::ostream &os, int depth);
    void draw_vector(
        std::ostream &os, int depth,
        std::shared_ptr<std::vector<std::shared_ptr<SyntaxNode>>> vec);
};

typedef std::shared_ptr<SyntaxNode> SyntaxNodePointer;
typedef std::shared_ptr<std::vector<SyntaxNodePointer>> SyntaxNodePointers;

class NodeVarRef : public SyntaxNode {
};

class NodeKeyword : public SyntaxNode {
   public:
    NodeKeyword(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeId : public NodeVarRef {
   public:
    NodeId(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeType : public SyntaxNode {
   public:
    NodeType() {}
    void draw(std::ostream &os, int depth) override;
};

typedef std::shared_ptr<NodeType> NodeTypePointer;

class NodeSimpleType : public NodeType {
   public:
    NodeSimpleType(SyntaxNodePointer type_id) : type_id(std::move(type_id)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer type_id;
};

class NodeArrayIndexRange : public NodeType {
   public:
    NodeArrayIndexRange(SyntaxNodePointer exp1, SyntaxNodePointer exp2)
        : exp1(std::move(exp1)), exp2(std::move(exp2)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer exp1;
    SyntaxNodePointer exp2;
};

class NodeArrayType : public NodeType {
   public:
    NodeArrayType(SyntaxNodePointer type, SyntaxNodePointers range)
        : type(std::move(type)), range(std::move(range)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer type;
    SyntaxNodePointers range;
};

class NodeFieldSelection : public NodeType {
   public:
    NodeFieldSelection(SyntaxNodePointers idents, SyntaxNodePointer type)
        : idents(std::move(idents)), type(std::move(type)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers idents;
    SyntaxNodePointer type;
};

class NodeRecordType : public NodeType {
   public:
    NodeRecordType(SyntaxNodePointers fields) : fields(std::move(fields)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers fields;
};

class NodeProgram : public SyntaxNode {
   public:
    NodeProgram(SyntaxNodePointer name, SyntaxNodePointer block)
        : name(std::move(name)), block(std::move(block)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer name;
    SyntaxNodePointer block;
};

class NodeProgramBlock : public SyntaxNode {
   public:
    NodeProgramBlock(SyntaxNodePointers declarations,
                     SyntaxNodePointer compound_statement)
        : declarations(std::move(declarations)),
          compound_statement(std::move(compound_statement)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers declarations;
    SyntaxNodePointer compound_statement;
};

class NodeVarDecls : public SyntaxNode {
   public:
    explicit NodeVarDecls(SyntaxNodePointers consts)
        : consts(std::move(consts)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers consts;
};

class NodeVarDecl : public SyntaxNode {
   public:
    NodeVarDecl(SyntaxNodePointers ids, SyntaxNodePointer type,
                SyntaxNodePointer exp)
        : ids(std::move(ids)), type(std::move(type)), exp(std::move(exp)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers ids;
    SyntaxNodePointer type;
    SyntaxNodePointer exp;  // may be nullptr
};

class NodeConstDecls : public SyntaxNode {
   public:
    explicit NodeConstDecls(SyntaxNodePointers consts)
        : consts(std::move(consts)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers consts;
};

class NodeConstDecl : public SyntaxNode {
   public:
    NodeConstDecl(SyntaxNodePointer id, SyntaxNodePointer type,
                  SyntaxNodePointer exp)
        : id(std::move(id)), type(std::move(type)), exp(std::move(exp)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointer type;  // may be nullptr
    SyntaxNodePointer exp;
};

class NodeTypeDecls : public SyntaxNode {
   public:
    explicit NodeTypeDecls(SyntaxNodePointers types)
        : types(std::move(types)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers types;
};

class NodeTypeDecl : public SyntaxNode {
   public:
    NodeTypeDecl(SyntaxNodePointer id, SyntaxNodePointer type)
        : id(std::move(id)), type(std::move(type)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointer type;
};

class NodeFormalParamSection : public SyntaxNode {
   public:
    NodeFormalParamSection(SyntaxNodePointer modifier,
                           SyntaxNodePointers idents, SyntaxNodePointer type)
        : modifier(std::move(modifier)),
          idents(std::move(idents)),
          type(std::move(type)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer modifier;  // may be nullptr
    SyntaxNodePointers idents;
    SyntaxNodePointer type;
};

class NodeHeaderProcedureDecl : public SyntaxNode {
   public:
    NodeHeaderProcedureDecl(SyntaxNodePointer ident, SyntaxNodePointers params)
        : ident(std::move(ident)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer ident;
    SyntaxNodePointers params;
};

class NodeProcedureDecl : public SyntaxNode {
   public:
    NodeProcedureDecl(SyntaxNodePointer header, SyntaxNodePointer block)
        : header(std::move(header)), block(std::move(block)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer header;
    SyntaxNodePointer block;
};

class NodeHeaderFunctionDecl : public SyntaxNode {
   public:
    NodeHeaderFunctionDecl(SyntaxNodePointer ident, SyntaxNodePointers params,
                           SyntaxNodePointer type)
        : ident(std::move(ident)),
          params(std::move(params)),
          type(std::move(type)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer ident;
    SyntaxNodePointers params;
    SyntaxNodePointer type;
};

class NodeFunctionDecl : public SyntaxNode {
   public:
    NodeFunctionDecl(SyntaxNodePointer header, SyntaxNodePointer block)
        : header(std::move(header)), block(std::move(block)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer header;
    SyntaxNodePointer block;
};

class NodeStatement : public SyntaxNode {};

class NodeCompoundStatement : public SyntaxNode {
   public:
    NodeCompoundStatement(SyntaxNodePointers stmts) : stmts(std::move(stmts)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers stmts;
};

class NodeForStatement : public SyntaxNode {
   public:
    NodeForStatement(SyntaxNodePointer param, SyntaxNodePointer start_exp,
                     SyntaxNodePointer dir, SyntaxNodePointer end_exp,
                     SyntaxNodePointer op)
        : param(std::move(param)),
          start_exp(std::move(start_exp)),
          dir(std::move(dir)),
          end_exp(std::move(end_exp)),
          op(std::move(op)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer param;
    SyntaxNodePointer start_exp;
    SyntaxNodePointer dir;
    SyntaxNodePointer end_exp;
    SyntaxNodePointer op;
};

class NodeWhileStatement : public SyntaxNode {
   public:
    NodeWhileStatement(SyntaxNodePointer bool_exp, SyntaxNodePointer op)
        : bool_exp(std::move(bool_exp)),
          op(std::move(op)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer bool_exp;
    SyntaxNodePointer op;
};

// class NodeDoWhileStatement : public SyntaxNode {
//     SyntaxNodePointer bool_exp;
//     SyntaxNodePointer op;
// };
//

class NodeIfStatement : public SyntaxNode {
   public:
    NodeIfStatement(SyntaxNodePointer bool_exp, SyntaxNodePointer op,
                    SyntaxNodePointer else_op)
        : bool_exp(std::move(bool_exp)),
          op(std::move(op)),
          else_op(std::move(else_op)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer bool_exp;
    SyntaxNodePointer op;
    SyntaxNodePointer else_op;
};

class NodeAssigmentStatement : public SyntaxNode {
   public:
    NodeAssigmentStatement(Token op, SyntaxNodePointer exp1,
                           SyntaxNodePointer exp2)
        : op(std::move(op)), exp1(std::move(exp1)), exp2(std::move(exp2)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token op;
    SyntaxNodePointer exp1;
    SyntaxNodePointer exp2;
};

// class NodeExpression : public SyntaxNode {
//    public:
//     NodeExpression() = default;
//     void draw(std::ostream &os, int depth) override;
// };

class NodeBinOp : public SyntaxNode {
   public:
    NodeBinOp(Token token, SyntaxNodePointer left, SyntaxNodePointer right)
        : token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
    SyntaxNodePointer left;
    SyntaxNodePointer right;
};

class NodeUnOp : public SyntaxNode {
   public:
    ~NodeUnOp() override = default;
    NodeUnOp(Token token, SyntaxNodePointer operand)
        : SyntaxNode(), token(std::move(token)), operand(std::move(operand)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
    SyntaxNodePointer operand;
};

class NodeRelOp : public SyntaxNode {
   public:
    NodeRelOp(Token token, SyntaxNodePointer left, SyntaxNodePointer right)
        : token(std::move(token)),
          left(std::move(left)),
          right(std::move(right)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
    SyntaxNodePointer left;
    SyntaxNodePointer right;
};

class NodeNumber : public SyntaxNode {
   public:
    NodeNumber(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeString : public SyntaxNode {
   public:
    NodeString(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeNil : public SyntaxNode {
   public:
    NodeNil(Token token) : token(std::move(token)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    Token token;
};

class NodeFuncCall : public SyntaxNode {
   public:
    NodeFuncCall(SyntaxNodePointer id, SyntaxNodePointers params)
        : id(std::move(id)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointers params;
};

class NodeArrayAccess : public NodeVarRef {
   public:
    NodeArrayAccess(SyntaxNodePointer id, SyntaxNodePointers params)
        : id(std::move(id)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointers params;
};

class NodeRecordAccess : public NodeVarRef {
   public:
    NodeRecordAccess(SyntaxNodePointer id, SyntaxNodePointer field)
        : id(std::move(id)), field(std::move(field)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointer field;
};

class NodeSetConstuctor : public SyntaxNode {
   public:
    NodeSetConstuctor(SyntaxNodePointer id, SyntaxNodePointers params)
        : id(std::move(id)), params(std::move(params)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer id;
    SyntaxNodePointers params;
};

class NodeSetElement : public SyntaxNode {};

class NodeSetSimpleElement : public NodeSetElement {
   public:
    NodeSetSimpleElement(SyntaxNodePointer exp) : exp(std::move(exp)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer exp;
};

class NodeSetRangeElement : public NodeSetElement {
   public:
    NodeSetRangeElement(SyntaxNodePointer exp1, SyntaxNodePointer exp2)
        : exp1(std::move(exp1)), exp2(std::move(exp2)) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointer exp1;
    SyntaxNodePointer exp2;
};

class NodeSetConstructor : public SyntaxNode {
   public:
    NodeSetConstructor(SyntaxNodePointers &set_elements)
        : set_elements(set_elements) {}
    void draw(std::ostream &os, int depth) override;

   private:
    SyntaxNodePointers set_elements;
};

}  // namespace parser

#endif  // PARSER_NODE_H
