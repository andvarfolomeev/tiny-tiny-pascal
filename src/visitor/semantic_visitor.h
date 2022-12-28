#ifndef VISITOR_SEMANTIC_VISITOR_H
#define VISITOR_SEMANTIC_VISITOR_H

#include "base_visitor.h"

namespace visitor {

class SemanticVisitor : public BaseVisitor {
   public:
    void visit(NodeKeyword* node) override;
    void visit(NodeBlock* node) override;
    void visit(NodeVarDecl* node) override;
    void visit(NodeVarDecls* node) override;
    void visit(NodeConstDecl* node) override;
    void visit(NodeConstDecls* node) override;
    void visit(NodeTypeDecl* node) override;
    void visit(NodeTypeDecls* node) override;
    void visit(NodeFormalParamSection* node) override;
    void visit(NodeProcedureDecl* node) override;
    void visit(NodeFunctionDecl* node) override;
    void visit(NodeId* node) override;
    void visit(NodeBoolean* node) override;
    void visit(NodeBinOp* node) override;
    void visit(NodeUnOp* node) override;
    void visit(NodeRelOp* node) override;
    void visit(NodeNumber* node) override;
    void visit(NodeCast* node) override;
    void visit(NodeString* node) override;
    void visit(NodeFuncCall* node) override;
    void visit(NodeArrayAccess* node) override;
    void visit(NodeRecordAccess* node) override;
    void visit(NodeSetElement* node) override;
    void visit(NodeSetConstructor* node) override;
    void visit(NodeProgram* node) override;
    void visit(NodeCallStatement* node) override;
    void visit(NodeCompoundStatement* node) override;
    void visit(NodeForStatement* node) override;
    void visit(NodeWhileStatement* node) override;
    void visit(NodeIfStatement* node) override;
    void visit(NodeAssigmentStatement* node) override;
    void visit(NodeSimpleType* node) override;
    void visit(NodeRange* node) override;
    void visit(NodeArrayType* node) override;
    void visit(NodeFieldSelection* node) override;
    void visit(NodeRecordType* node) override;

   protected:
};

}  // namespace visitor

#endif  // VISITOR_SEMANTIC_VISITOR_H
