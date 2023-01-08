#ifndef VISITOR_BASE_VISITOR_H
#define VISITOR_BASE_VISITOR_H

#include <memory>

namespace parser {
class NodeKeyword;
class NodeBlock;
class NodeVarDecl;
class NodeVarDecls;
class NodeConstDecl;
class NodeConstDecls;
class NodeTypeDecl;
class NodeTypeDecls;
class NodeFormalParamSection;
class NodeProcedureDecl;
class NodeFunctionDecl;
class NodeVarRef;
class NodeId;
class NodeBoolean;
class NodeBinOp;
class NodeUnOp;
class NodeRelOp;
class NodeNumber;
class NodeCast;
class NodeString;
class NodeFuncCall;
class NodeArrayAccess;
class NodeRecordAccess;
class NodeProgram;
class NodeCallStatement;
class NodeCompoundStatement;
class NodeForStatement;
class NodeWhileStatement;
class NodeIfStatement;
class NodeAssigmentStatement;
class NodeType;
class NodeSimpleType;
class NodeRange;
class NodeArrayType;
class NodeFieldSelection;
class NodeRecordType;
};  // namespace parser

namespace visitor {
using namespace parser;
class BaseVisitor {
   public:
    virtual void visit(NodeKeyword* node) = 0;
    virtual void visit(NodeBlock* node) = 0;
    virtual void visit(NodeVarDecl* node) = 0;
    virtual void visit(NodeVarDecls* node) = 0;
    virtual void visit(NodeConstDecl* node) = 0;
    virtual void visit(NodeConstDecls* node) = 0;
    virtual void visit(NodeTypeDecl* node) = 0;
    virtual void visit(NodeTypeDecls* node) = 0;
    virtual void visit(NodeFormalParamSection* node) = 0;
    virtual void visit(NodeProcedureDecl* node) = 0;
    virtual void visit(NodeFunctionDecl* node) = 0;
    virtual void visit(NodeId* node) = 0;
    virtual void visit(NodeBoolean* node) = 0;
    virtual void visit(NodeBinOp* node) = 0;
    virtual void visit(NodeUnOp* node) = 0;
    virtual void visit(NodeRelOp* node) = 0;
    virtual void visit(NodeNumber* node) = 0;
    virtual void visit(NodeCast* node) = 0;
    virtual void visit(NodeString* node) = 0;
    virtual void visit(NodeFuncCall* node) = 0;
    virtual void visit(NodeArrayAccess* node) = 0;
    virtual void visit(NodeRecordAccess* node) = 0;
    virtual void visit(NodeProgram* node) = 0;
    virtual void visit(NodeCallStatement* node) = 0;
    virtual void visit(NodeCompoundStatement* node) = 0;
    virtual void visit(NodeForStatement* node) = 0;
    virtual void visit(NodeWhileStatement* node) = 0;
    virtual void visit(NodeIfStatement* node) = 0;
    virtual void visit(NodeAssigmentStatement* node) = 0;
    virtual void visit(NodeSimpleType* node) = 0;
    virtual void visit(NodeRange* node) = 0;
    virtual void visit(NodeArrayType* node) = 0;
    virtual void visit(NodeFieldSelection* node) = 0;
    virtual void visit(NodeRecordType* node) = 0;
};
}  // namespace visitor

#endif  // VISITOR_BASE_VISITOR_H
