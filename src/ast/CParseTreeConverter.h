#pragma once
#include <vector>
#include "antlr4-runtime.h"
#include "SOMParserBaseVisitor.h"
#include "ASTNodes.h"

using namespace antlr_som;

namespace som {


	class CParseTreeConverter : public SOMParserBaseVisitor {
	public:
		antlrcpp::Any visitClassdef(SOMParser::ClassdefContext* ctx) override;
		
		antlrcpp::Any visitString(SOMParser::StringContext* ctx) override;
		antlrcpp::Any visitVariable(SOMParser::VariableContext* ctx) override;
		antlrcpp::Any visitIdentifier(SOMParser::IdentifierContext* ctx) override;
		antlrcpp::Any visitInstanceFields(SOMParser::InstanceFieldsContext* ctx) override;
		antlrcpp::Any visitMethod(SOMParser::MethodContext* ctx) override;
		antlrcpp::Any visitMethodBlock(SOMParser::MethodBlockContext* ctx) override;
		antlrcpp::Any visitClassFields(SOMParser::ClassFieldsContext* ctx) override;
		antlrcpp::Any visitClassMethod(SOMParser::ClassMethodContext* ctx) override;
		antlrcpp::Any visitSuperclass(SOMParser::SuperclassContext* ctx) override;

		antlrcpp::Any visitPattern(SOMParser::PatternContext* ctx) override;
		antlrcpp::Any visitUnaryPattern(SOMParser::UnaryPatternContext* ctx) override;
		antlrcpp::Any visitBinaryPattern(SOMParser::BinaryPatternContext* ctx) override;
		antlrcpp::Any visitBinarySelector(SOMParser::BinarySelectorContext* ctx) override;
		antlrcpp::Any visitKeywordPattern(SOMParser::KeywordPatternContext* ctx) override;
		antlrcpp::Any visitKeyword(SOMParser::KeywordContext* ctx) override;

		antlrcpp::Any visitArgument(SOMParser::ArgumentContext* ctx) override;

		antlrcpp::Any visitLocalDefs(SOMParser::LocalDefsContext* ctx) override;
		antlrcpp::Any visitBlockBody(SOMParser::BlockBodyContext* ctx) override;
		antlrcpp::Any visitNestedBlock(SOMParser::NestedBlockContext* ctx) override;
		antlrcpp::Any visitBlockPattern(SOMParser::BlockPatternContext* ctx) override;
		antlrcpp::Any visitBlockArguments(SOMParser::BlockArgumentsContext* ctx) override;
		antlrcpp::Any visitBlockContents(SOMParser::BlockContentsContext* ctx) override;

		//antlrcpp::Any visitLiteral(SOMParser::LiteralContext* ctx) override;
		antlrcpp::Any visitLiteralArray(SOMParser::LiteralArrayContext* ctx) override;
		antlrcpp::Any visitLiteralString(SOMParser::LiteralStringContext* ctx) override;
		antlrcpp::Any visitLiteralNumber(SOMParser::LiteralNumberContext* ctx) override;
		antlrcpp::Any createLiteralDecimal(SOMParser::LiteralDecimalContext* ctx, bool isNegative);
		antlrcpp::Any createLiteralInteger(SOMParser::LiteralIntegerContext* ctx, bool isNegative);
		antlrcpp::Any createLiteralDouble(SOMParser::LiteralDoubleContext* ctx, bool isNegative);

		antlrcpp::Any visitPrimary(SOMParser::PrimaryContext* ctx) override;
		antlrcpp::Any visitNestedTerm(SOMParser::NestedTermContext* ctx) override;
		antlrcpp::Any visitExpression(SOMParser::ExpressionContext* ctx) override;
		antlrcpp::Any visitAssignation(SOMParser::AssignationContext* ctx) override;
		antlrcpp::Any visitAssignments(SOMParser::AssignmentsContext* ctx) override;
		antlrcpp::Any visitAssignment(SOMParser::AssignmentContext* ctx) override;
		antlrcpp::Any visitEvaluation(SOMParser::EvaluationContext* ctx) override;

		antlrcpp::Any visitMessages(SOMParser::MessagesContext* ctx) override;
		antlrcpp::Any visitUnaryMessage(SOMParser::UnaryMessageContext* ctx) override;
		antlrcpp::Any visitUnarySelector(SOMParser::UnarySelectorContext* ctx) override;
		antlrcpp::Any visitBinaryMessage(SOMParser::BinaryMessageContext* ctx) override;
		antlrcpp::Any visitBinaryOperand(SOMParser::BinaryOperandContext* ctx) override;
		antlrcpp::Any visitKeywordMessage(SOMParser::KeywordMessageContext* ctx) override;
		antlrcpp::Any visitFormula(SOMParser::FormulaContext* ctx) override;
		antlrcpp::Any visitResult(SOMParser::ResultContext* ctx) override;

		//antlrcpp::Any visitPrimary(SOMParser::PrimaryContext* ctx) override;

	private:
		nodeVector* makeFieldsVector(const std::vector<SOMParser::VariableContext*>& variables);
		void appendBlockExpression(SOMParser::BlockBodyContext* ctx, nodeVector* expressions);
	};

}


