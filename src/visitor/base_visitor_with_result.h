#ifndef VISITOR_BASE_VISITOR_WITH_RESULT_H
#define VISITOR_BASE_VISITOR_WITH_RESULT_H

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

class SymbolVar;
class SymbolVarLocal;
class SymbolVarGlobal;

namespace visitor {
using namespace parser;
class BaseVisitorWithResult {
   public:
    virtual void visit(NodeKeyword* node, bool result = true) = 0;
    virtual void visit(NodeBlock* node, bool result = true) = 0;

    virtual void visit(SymbolVar* sym, bool result = true) = 0;
    virtual void visit(SymbolVarLocal* sym, bool result = true) = 0;
    virtual void visit(SymbolVarGlobal* sym, bool result = true) = 0;
    virtual void visit(NodeVarDecl* node, bool result = true) = 0;
    virtual void visit(NodeVarDecls* node, bool result = true) = 0;
    virtual void visit(NodeConstDecl* node, bool result = true) = 0;
    virtual void visit(NodeConstDecls* node, bool result = true) = 0;
    virtual void visit(NodeTypeDecl* node, bool result = true) = 0;
    virtual void visit(NodeTypeDecls* node, bool result = true) = 0;
    virtual void visit(NodeFormalParamSection* node, bool result = true) = 0;
    virtual void visit(NodeProcedureDecl* node, bool result = true) = 0;
    virtual void visit(NodeFunctionDecl* node, bool result = true) = 0;
    virtual void visit(NodeId* node, bool result = true) = 0;
    virtual void visit(NodeBoolean* node, bool result = true) = 0;
    virtual void visit(NodeBinOp* node, bool result = true) = 0;
    virtual void visit(NodeUnOp* node, bool result = true) = 0;
    virtual void visit(NodeRelOp* node, bool result = true) = 0;
    virtual void visit(NodeNumber* node, bool result = true) = 0;
    virtual void visit(NodeCast* node, bool result = true) = 0;
    virtual void visit(NodeString* node, bool result = true) = 0;
    virtual void visit(NodeFuncCall* node, bool result = true) = 0;
    virtual void visit(NodeArrayAccess* node, bool result = true) = 0;
    virtual void visit(NodeRecordAccess* node, bool result = true) = 0;
    virtual void visit(NodeProgram* node, bool result = true) = 0;
    virtual void visit(NodeCallStatement* node, bool result = true) = 0;
    virtual void visit(NodeCompoundStatement* node, bool result = true) = 0;
    virtual void visit(NodeForStatement* node, bool result = true) = 0;
    virtual void visit(NodeWhileStatement* node, bool result = true) = 0;
    virtual void visit(NodeIfStatement* node, bool result = true) = 0;
    virtual void visit(NodeAssigmentStatement* node, bool result = true) = 0;
    virtual void visit(NodeSimpleType* node, bool result = true) = 0;
    virtual void visit(NodeRange* node, bool result = true) = 0;
    virtual void visit(NodeArrayType* node, bool result = true) = 0;
    virtual void visit(NodeFieldSelection* node, bool result = true) = 0;
    virtual void visit(NodeRecordType* node, bool result = true) = 0;
};
}  // namespace visitor

#endif  // VISITOR_BASE_VISITOR_WITH_RESULT_H
