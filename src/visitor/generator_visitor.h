#ifndef GENERATOR_SEMANTIC_VISITOR_H
#define GENERATOR_SEMANTIC_VISITOR_H

#include <ostream>
#include <vector>

#include "../generator/generator.h"
#include "../scanner/token.h"
#include "base_visitor_with_result.h"

class SymbolVar;
class SymbolVarLocal;
class SymbolVarGlobal;

namespace visitor {
class GeneratorVisitor : public BaseVisitorWithResult {
   public:
    explicit GeneratorVisitor(Generator& generator) : g(generator) {}
    ~GeneratorVisitor() = default;

    void visit(NodeKeyword* node, bool result = true) override;
    void visit(NodeBlock* node, bool result = true) override;

    void visit(SymbolVar* sym, bool result = true);
    void visit(SymbolVarLocal* sym, bool result = true);
    void visit(SymbolVarGlobal* sym, bool result = true);

    void visit(NodeVarDecl* node, bool result = true) override;
    void visit(NodeVarDecls* node, bool result = true) override;
    void visit(NodeConstDecl* node, bool result = true) override;
    void visit(NodeConstDecls* node, bool result = true) override;
    void visit(NodeTypeDecl* node, bool result = true) override;
    void visit(NodeTypeDecls* node, bool result = true) override;
    void visit(NodeFormalParamSection* node, bool result = true) override;
    void visit(NodeProcedureDecl* node, bool result = true) override;
    void visit(NodeFunctionDecl* node, bool result = true) override;
    void visit(NodeId* node, bool result = true) override;
    void visit(NodeBoolean* node, bool result = true) override;
    void visit(NodeBinOp* node, bool result = true) override;
    void visit(NodeUnOp* node, bool result = true) override;
    void visit(NodeRelOp* node, bool result = true) override;
    void visit(NodeNumber* node, bool result = true) override;
    void visit(NodeCast* node, bool result = true) override;
    void visit(NodeString* node, bool result = true) override;
    void visit(NodeFuncCall* node, bool result = true) override;
    void visit(NodeArrayAccess* node, bool result = true) override;
    void visit(NodeRecordAccess* node, bool result = true) override;
    void visit(NodeProgram* node, bool result = true) override;
    void visit(NodeCallStatement* node, bool result = true) override;
    void visit(NodeCompoundStatement* node, bool result = true) override;
    void visit(NodeForStatement* node, bool result = true) override;
    void visit(NodeWhileStatement* node, bool result = true) override;
    void visit(NodeIfStatement* node, bool result = true) override;
    void visit(NodeAssigmentStatement* node, bool result = true) override;
    void visit(NodeSimpleType* node, bool result = true) override;
    void visit(NodeRange* node, bool result = true) override;
    void visit(NodeArrayType* node, bool result = true) override;
    void visit(NodeFieldSelection* node, bool result = true) override;
    void visit(NodeRecordType* node, bool result = true) override;

   protected:
    Generator& g;
};

}  // namespace visitor

#endif  // GENERATOR_SEMANTIC_VISITOR_H
