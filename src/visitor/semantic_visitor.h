#ifndef VISITOR_SEMANTIC_VISITOR_H
#define VISITOR_SEMANTIC_VISITOR_H

#include <sstream>

#include "../exception.h"
#include "../scanner/token.h"
#include "../symbol_table/symbol_table_stack.h"
#include "../symbol_table/symbol_type_array.h"
#include "../symbol_table/symbol_var.h"
#include "base_visitor.h"

namespace visitor {
class SemanticVisitor : public BaseVisitor {
   public:
    explicit SemanticVisitor(std::shared_ptr<SymbolTableStack> sym_table_stack)
        : sym_table_stack(std::move(sym_table_stack)) {}
    SemanticVisitor() : sym_table_stack(std::make_shared<SymbolTableStack>()) {
        sym_table_stack->alloc_with_builtin();
    }

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

    std::shared_ptr<SymbolTableStack> get_sym_table_stack();

   private:
    std::shared_ptr<SymbolType> get_symbol_type(
        const std::shared_ptr<NodeType>& type);
    std::shared_ptr<SymbolType> get_symbol_type(
        const std::shared_ptr<NodeRecordType>& type);
    std::shared_ptr<SymbolType> get_symbol_type(
        const std::shared_ptr<NodeArrayType>& array_type);
    std::shared_ptr<Symbol> get_symbol_by_id(NodeId* id);
    std::shared_ptr<SymbolType> get_symbol_type_by_id(NodeId* id);
    static std::shared_ptr<SymbolType> solve_casting(NodeBinOp* node);
    static void solve_casting(std::shared_ptr<SymbolType> left_st,
                              std::shared_ptr<NodeExpression>& right);
    static void check_type_exist(const std::shared_ptr<NodeExpression>& exp);

   protected:
    std::shared_ptr<SymbolTableStack> sym_table_stack;
};

}  // namespace visitor

#endif  // VISITOR_SEMANTIC_VISITOR_H
