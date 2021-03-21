#pragma once
#include <string>
#include <memory>
#include <vector>
#include "antlr4-runtime.h"
#include "ASTAbstractVisitor.h"

#define VISITABLE virtual void accept(CAstAbstractVisitor& visitor) final { visitor.visit(this); }

namespace som {

	struct ASTNode {
		virtual void accept(CAstAbstractVisitor& visitor) = 0;
	};

	typedef std::unique_ptr<ASTNode> nodePtr;
	typedef std::vector<nodePtr> nodeVector;

	template <class T, class... Args>
	ASTNode* makeNode(Args&&... args)
	{
		return new T(std::forward<Args>(args)...);
	}

	struct Class : ASTNode {
		Class(std::string identifier, nodeVector& instanceFields, nodeVector& instanceMethods,
			nodeVector& classFields, nodeVector& classMethods) : m_identifier(identifier), m_instanceFields(std::move(instanceFields)),
			m_instanceMethods(std::move(instanceMethods)), m_classFields(std::move(classFields)), m_classMethods(std::move(classMethods))
		{}

		std::string m_identifier;
		nodePtr m_superclass;
		nodeVector m_instanceFields;
		nodeVector m_instanceMethods;
		nodeVector m_classFields;
		nodeVector m_classMethods;
		VISITABLE
	};

	// Methods and message patterns
	struct Method : ASTNode {
		Method(nodePtr pattern, nodePtr block) :
			m_pattern(std::move(pattern)), m_methodBlock(std::move(block))
		{}

		nodePtr m_pattern;
		nodePtr m_methodBlock;
		VISITABLE
	};

	struct Block : ASTNode {
		Block(nodeVector& localDefs, nodeVector& expressions) : m_localDefs(std::move(localDefs)), m_expressions(std::move(localDefs)) {}
		nodeVector m_localDefs;
		nodeVector m_expressions;
		VISITABLE
	};
	struct NestedBlock : ASTNode {
		NestedBlock(nodeVector& args, nodePtr block) : m_arguments(std::move(args)), m_block(std::move(block)) {}
		nodeVector m_arguments;
		nodePtr m_block;
		VISITABLE
	};


	struct UnaryPattern : ASTNode {
		UnaryPattern(std::string identifier) : m_identifier(std::move(identifier)) {}
		std::string m_identifier;
		VISITABLE
	};

	struct BinaryPattern : ASTNode {
		BinaryPattern(std::string identifier, nodePtr argument) : m_identifier(std::move(identifier)), m_argument(std::move(argument)) {}
		std::string m_identifier;		
		nodePtr m_argument;
		VISITABLE
	};

	struct KeywordPattern : ASTNode {
		KeywordPattern(nodeVector& keywords, nodeVector& args) : m_keywords(std::move(keywords)), m_arguments(std::move(args)) {}
		nodeVector m_keywords;
		nodeVector m_arguments;
		VISITABLE
	};

	struct Keyword : ASTNode {
		Keyword(std::string keyword) : m_keyword(std::move(keyword)) {}
		std::string m_keyword;
		VISITABLE
	};

	struct KeywordWithArgs : ASTNode {
		KeywordWithArgs(std::string identifier, nodePtr argument) : m_identifier(std::move(identifier)), m_argument(std::move(argument)) {}
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
		KeywordSelector(nodeVector& keywords) : m_keywords(std::move(keywords)) {}
		nodeVector m_keywords;
		VISITABLE
	};

	// Message sends
	struct UnaryMessage : ASTNode {
		UnaryMessage(nodePtr selector) : m_unarySelector(std::move(selector)) {}
		nodePtr m_unarySelector;
		VISITABLE
	};

	struct BinaryMessage : ASTNode {
		BinaryMessage(nodePtr selector, nodePtr operand) : m_binarySelector(std::move(selector)), m_operand(std::move(operand)) {}
		nodePtr m_binarySelector;
		nodePtr m_operand;
		VISITABLE
	};
	struct BinaryOperand : ASTNode {
		BinaryOperand(nodePtr primary, nodeVector& messages) : m_primary(std::move(primary)), m_unaryMessages(std::move(messages)) {}
		nodePtr m_primary;
		nodeVector m_unaryMessages;
		VISITABLE
	};

	struct KeywordMessage : ASTNode {
		KeywordMessage(nodeVector& keywords, nodeVector& formulas) : m_keywords(std::move(keywords)), m_formulas(std::move(formulas)) {}
		nodeVector m_keywords;
		nodeVector m_formulas;
		VISITABLE
	};
	struct Formula : ASTNode {
		Formula(nodePtr arg, nodeVector& binaryMessage) : m_argument(std::move(arg)), m_binaryMessage(std::move(binaryMessage)) {}
		nodePtr m_argument;
		nodeVector m_binaryMessage;
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
		LiteralArray(nodeVector& literals) : m_literals(std::move(literals)) {}
		nodeVector m_literals;
		VISITABLE
	};

	struct Assignation : ASTNode {
		Assignation(nodeVector& variables, nodePtr value) : m_variables(std::move(variables)), m_value(std::move(value)) {}
		nodeVector m_variables;
		nodePtr m_value;
		VISITABLE
	};

	struct Evaluation : ASTNode {
		Evaluation(nodePtr primary, nodeVector& messages) : m_primary(std::move(primary)), m_messages(std::move(messages)) {}
		nodePtr m_primary;
		nodeVector m_messages;
		VISITABLE
	};

	struct Variable : ASTNode {
		Variable(std::string identifier) : m_identifier(identifier) {}
		std::string m_identifier;
		VISITABLE
	};

	struct NestedTerm : ASTNode {
		NestedTerm(nodePtr exp) : m_expression(std::move(exp)) {}
		nodePtr m_expression;
		VISITABLE
	};

}
