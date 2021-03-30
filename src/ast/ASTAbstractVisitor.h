#pragma once

#include <any>

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
	struct KeywordWithArgs;
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
		virtual std::any visit(Method* method) = 0;
		virtual std::any visit(Class* classNode) = 0;
		virtual std::any visit(Block* block) = 0;
		virtual std::any visit(NestedBlock* nestedBlock) = 0;
		virtual std::any visit(UnaryPattern* unaryPattern) = 0;
		virtual std::any visit(BinaryPattern* binaryPattern) = 0;
		virtual std::any visit(KeywordPattern* keywordPattern) = 0;
		virtual std::any visit(Keyword* keyword) = 0;
		virtual std::any visit(KeywordWithArgs* keyword) = 0;

		virtual std::any visit(UnarySelector* unarySelector) = 0;
		virtual std::any visit(BinarySelector* binarySelector) = 0;
		virtual std::any visit(KeywordSelector* keywordSelector) = 0;

		virtual std::any visit(UnaryMessage* unaryMessage) = 0;
		virtual std::any visit(BinaryMessage* binaryMessage) = 0;
		virtual std::any visit(BinaryOperand* binaryOperand) = 0;
		virtual std::any visit(KeywordMessage* keywordMessage) = 0;
		
		virtual std::any visit(Formula* formula) = 0;

		virtual std::any visit(LiteralInteger* litInteger) = 0;
		virtual std::any visit(LiteralString* litString) = 0;
		virtual std::any visit(LiteralArray* litArray) = 0;
		virtual std::any visit(LiteralDouble* litDouble) = 0;
		
		virtual std::any visit(Assignation* assignation) = 0;
		virtual std::any visit(Evaluation* evaluation) = 0;
		virtual std::any visit(Variable* variable) = 0;
		virtual std::any visit(NestedTerm* nestedTerm) = 0;
	};

}
