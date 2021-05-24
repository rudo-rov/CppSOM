#pragma once
#include <string>
#include <memory>
#include <vector>
#include <any>
#include "antlr4-runtime.h"
#include "ASTAbstractVisitor.h"

#define VISITABLE virtual std::any accept(CAstAbstractVisitor& visitor) final { return visitor.visit(this); }

namespace som {

	struct ASTNode {
		virtual std::any accept(CAstAbstractVisitor& visitor) = 0;
		
	};

	typedef std::unique_ptr<ASTNode> nodePtr;
	typedef std::vector<nodePtr> nodeVector;
	typedef std::unique_ptr<nodeVector> nodeVectorPtr;

	template <class T, class... Args>
	ASTNode* makeNode(Args&&... args)
	{
		return new T(std::forward<Args>(args)...);
	}

	struct Class : ASTNode {
		Class(std::string identifier, std::string superclass, nodeVector* instanceFields, nodeVector* instanceMethods,
			nodeVector* classFields, nodeVector* classMethods) : m_identifier(identifier), m_superclass(superclass), m_instanceFields(instanceFields),
			m_instanceMethods(instanceMethods), m_classFields(classFields), m_classMethods(classMethods)
		{}

		std::string m_identifier;
		std::string m_superclass;
		nodeVectorPtr m_instanceFields;
		nodeVectorPtr m_instanceMethods;
		nodeVectorPtr m_classFields;
		nodeVectorPtr m_classMethods;
		VISITABLE
	};

	// Methods and message patterns
	struct Method : ASTNode {
		Method(ASTNode* pattern, ASTNode* block, bool primitive) :
			m_pattern(pattern), m_methodBlock(block), m_primitive(primitive)
		{}

		nodePtr m_pattern;
		nodePtr m_methodBlock;
		bool m_primitive;
		VISITABLE
	};

	struct Block : ASTNode {
		Block(nodeVector* localDefs, nodeVector* expressions) : m_localDefs(localDefs), m_expressions(expressions) {}
		nodeVectorPtr m_localDefs;
		nodeVectorPtr m_expressions;
		VISITABLE
	};
	struct NestedBlock : ASTNode {
		NestedBlock(nodeVector* args, ASTNode* block) : m_arguments(args), m_block(block) {}
		nodeVectorPtr m_arguments;
		nodePtr m_block;
		VISITABLE
	};


	struct UnaryPattern : ASTNode {
		UnaryPattern(std::string identifier) : m_identifier(std::move(identifier)) {}
		std::string m_identifier;
		VISITABLE
	};

	struct BinaryPattern : ASTNode {
		BinaryPattern(std::string identifier, ASTNode* argument) : m_identifier(std::move(identifier)), m_argument(argument) {}
		std::string m_identifier;		
		nodePtr m_argument;
		VISITABLE
	};

	struct KeywordPattern : ASTNode {
		KeywordPattern(nodeVector* keywords, nodeVector* args) : m_keywords(keywords), m_arguments(args) {}
		nodeVectorPtr m_keywords;
		nodeVectorPtr m_arguments;
		VISITABLE
	};

	struct Keyword : ASTNode {
		Keyword(std::string keyword) : m_keyword(std::move(keyword)) {}
		std::string m_keyword;
		VISITABLE
	};

	struct KeywordWithArgs : ASTNode {
		KeywordWithArgs(std::string identifier, ASTNode* argument) : m_identifier(std::move(identifier)), m_argument(argument) {}
		std::string m_identifier;
		nodePtr m_argument;
		VISITABLE
	};

	// Method selectors
	struct UnarySelector : ASTNode {
		UnarySelector(std::string identifier) : m_identifier(identifier) {}
		std::string m_identifier;
		VISITABLE
	};

	struct KeywordSelector : ASTNode {
		KeywordSelector(nodeVector* keywords) : m_keywords(keywords) {}
		nodeVectorPtr m_keywords;
		VISITABLE
	};

	// Message sends
	struct UnaryMessage : ASTNode {
		UnaryMessage(ASTNode* selector) : m_unarySelector(selector) {}
		nodePtr m_unarySelector;
		VISITABLE
	};

	struct BinaryMessage : ASTNode {
		BinaryMessage(std::string selector, ASTNode* operand) : m_identifier(selector), m_operand(operand) {}
		std::string m_identifier;
		nodePtr m_operand;
		VISITABLE
	};
	struct BinaryOperand : ASTNode {
		BinaryOperand(ASTNode* primary, nodeVector* messages) : m_primary(primary), m_unaryMessages(messages) {}
		nodePtr m_primary;
		nodeVectorPtr m_unaryMessages;
		VISITABLE
	};

	struct KeywordMessage : ASTNode {
		KeywordMessage(nodeVector* keywords, nodeVector* formulas) : m_keywords(keywords), m_formulas(formulas) {}
		nodeVectorPtr m_keywords;
		nodeVectorPtr m_formulas;
		VISITABLE
	};
	struct Formula : ASTNode {
		Formula(ASTNode* arg, nodeVector* binaryMessage) : m_argument(arg), m_binaryMessage(binaryMessage) {}
		nodePtr m_argument;
		nodeVectorPtr m_binaryMessage;
		VISITABLE
	};


	// Literals
	struct LiteralInteger : ASTNode {
		LiteralInteger(int32_t value) : m_value(value) {}
		int32_t m_value;
		VISITABLE
	};

	struct LiteralDouble : ASTNode {
		LiteralDouble(double value) : m_value(value) {}
		double m_value;
		VISITABLE
	};

	struct LiteralString : ASTNode {
		LiteralString(std::string value) : m_value(std::move(value)) {}
		std::string m_value;
		VISITABLE
	};

	struct LiteralArray : ASTNode {
		LiteralArray(nodeVector* literals) : m_literals(literals) {}
		nodeVectorPtr m_literals;
		VISITABLE
	};

	struct Assignation : ASTNode {
		Assignation(nodeVector* variables, ASTNode* value) : m_variables(variables), m_value(value) {}
		nodeVectorPtr m_variables;
		nodePtr m_value;
		VISITABLE
	};

	struct Evaluation : ASTNode {
		Evaluation(ASTNode* primary, nodeVector* messages) : m_primary(primary), m_messages(messages) {}
		nodePtr m_primary;
		nodeVectorPtr m_messages;
		VISITABLE
	};

	struct Result : ASTNode {
		Result(ASTNode* expression) : m_expression(expression) {} 
		nodePtr m_expression;
		VISITABLE
	};

	struct Variable : ASTNode {
		Variable(std::string identifier) : m_identifier(identifier) {}
		std::string m_identifier;
		VISITABLE
	};

	struct NestedTerm : ASTNode {
		NestedTerm(ASTNode* exp) : m_expression(exp) {}
		nodePtr m_expression;
		VISITABLE
	};

}
