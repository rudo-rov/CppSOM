#pragma once

#include <memory>
#include <string>
#include <vector>
#include <any>

#include "../ast/ASTAbstractVisitor.h"
#include "../ast/ASTNodes.h"
#include "Program.h"
#include "../interpretation/ScopeStack.h"

namespace som {

    class CBytecodeCompiler : public CAstAbstractVisitor {
    public:

        CBytecodeCompiler(Program* program) : m_program(program) {}
        
        std::any visit(ASTNode* node) { return node->accept(*this); }
        
        std::any visit(Class* classNode) override;
        std::any visit(Method* method) override;
        std::any visit(Block* block) override;
        std::any visit(NestedBlock* nestedBlock) override { return std::any(); }
        std::any visit(UnaryPattern* unaryPattern) override;
        std::any visit(BinaryPattern* binaryPattern) override;
        std::any visit(KeywordPattern* keywordPattern) override;
        std::any visit(Keyword* keyword) override;
        std::any visit(KeywordWithArgs* keyworrd) override { return std::any(); }

        std::any visit(UnarySelector* unarySelector) override { return std::any(); }
        std::any visit(BinarySelector* binarySelector) override { return std::any(); }
        std::any visit(KeywordSelector* keywordSelector) override { return std::any(); }

        std::any visit(UnaryMessage* unaryMessage) override;
        std::any visit(BinaryMessage* binaryMessage) override;
        std::any visit(BinaryOperand* binaryOperand) override { return std::any(); }
        std::any visit(KeywordMessage* keywordMessage) override { return std::any(); }

        std::any visit(Formula* formula) override { return std::any(); }
        
        std::any visit(LiteralInteger* litInteger) override;
        std::any visit(LiteralString* litString) override;
        std::any visit(LiteralArray* litArray) override { return std::any(); }
        std::any visit(LiteralDouble* litDouble) override;

        // Will return insVector* with compiled code
        std::any visit(Assignation* assignation) override;
        std::any visit(Evaluation* evaluation) override;
        std::any visit(Variable* variable) override;
        std::any visit(NestedTerm* nestedTerm) override;    
    private:
        
        
        Program* m_program;
        BCClass m_class;
        CScopeStack m_scopes;
        std::string m_lastError;

        void appendInstructions(insVector* first, insVector* second) const;
        ByteIns* resolveVariable(ASTNode* node);
    };

}