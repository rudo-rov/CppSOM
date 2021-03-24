#pragma once

#include <memory>

#include "../ast/ASTAbstractVisitor.h"
#include "../ast/ASTNodes.h"
#include "Program.h"

namespace som {

    class CBytecodeCompiler : public CAstAbstractVisitor {
    public:

        CBytecodeCompiler(Program* program) : m_program(program) {}
        
        void visit(Class* classNode) override;
        void visit(Method* method) override;
        void visit(Block* block) override {}
        void visit(NestedBlock* nestedBlock) override {}
        void visit(UnaryPattern* unaryPattern) override;
        void visit(BinaryPattern* binaryPattern) override;
        void visit(KeywordPattern* keywordPattern) override {}
        void visit(Keyword* keyword) override {}
        void visit(KeywordWithArgs* keyworrd) override {}

        void visit(UnarySelector* unarySelector) override {}
        void visit(BinarySelector* binarySelector) override {}
        void visit(KeywordSelector* keywordSelector) override {}

        void visit(UnaryMessage* unaryMessage) override {}
        void visit(BinaryMessage* binaryMessage) override {}
        void visit(BinaryOperand* binaryOperand) override {}
        void visit(KeywordMessage* keywordMessage) override {}

        void visit(Formula* formula) override {}
        
        void visit(LiteralInteger* litInteger) override;
        void visit(LiteralString* litString) override;
        void visit(LiteralArray* litArray) override {}
        void visit(LiteralDouble* litDouble) override;

        void visit(Assignation* assignation) override {}
        void visit(Evaluation* evaluation) override {}
        void visit(Variable* variable) override;
        void visit(NestedTerm* nestedTerm) override {}
    
    private:
        Program* m_program;
        BCClass m_class;


    };

}