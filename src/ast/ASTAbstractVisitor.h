#pragma once

namespace som {

	// Forward declaration of AST nodes to resolve circular reference
	struct Class;
	struct Method;
	struct Block;
	struct NestedBlock;
	struct UnaryPattern;
	struct BinaryPattern;
	struct KeywordPattern;
	struct Keyword;
	struct UnarySelector;
	struct BinarySelector;
	struct BinaryOperand;
	struct KeywordSelector;
	struct UnaryMessage;
	struct BinaryMessage;
	struct KeywordMessage;
	struct Formula;
	struct LiteralInteger;
	struct LiteralDouble;
	struct LiteralArray;
	struct LiteralString;
	struct Assignation;
	struct Evaluation;
	struct Variable;
	struct NestedTerm;
	
	class CAstAbstractVisitor {
	public:
		virtual void visit(Class* classNode) = 0;
		virtual void visit(Method* method) = 0;
		virtual void visit(Block* block) = 0;
		virtual void visit(NestedBlock* nestedBlock) = 0;
		virtual void visit(UnaryPattern* unaryPattern) = 0;
		virtual void visit(BinaryPattern* binaryPattern) = 0;
		virtual void visit(KeywordPattern* keywordPattern) = 0;
		virtual void visit(Keyword* keyword) = 0;

		virtual void visit(UnarySelector* unarySelector) = 0;
		virtual void visit(BinarySelector* binarySelector) = 0;
		virtual void visit(KeywordSelector* keywordSelector) = 0;

		virtual void visit(UnaryMessage* unaryMessage) = 0;
		virtual void visit(BinaryMessage* binaryMessage) = 0;
		virtual void visit(BinaryOperand* binaryOperand) = 0;
		virtual void visit(KeywordMessage* keywordMessage) = 0;
		
		virtual void visit(Formula* formula) = 0;

		virtual void visit(LiteralInteger* litInteger) = 0;
		virtual void visit(LiteralString* litString) = 0;
		virtual void visit(LiteralArray* litArray) = 0;
		virtual void visit(LiteralDouble* litDouble) = 0;
		
		virtual void visit(Assignation* assignation) = 0;
		virtual void visit(Evaluation* evaluation) = 0;
		virtual void visit(Variable* variable) = 0;
		virtual void visit(NestedTerm* nestedTerm) = 0;
	};

}
