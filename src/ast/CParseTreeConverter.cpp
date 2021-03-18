#include <string>
#include <cassert>

#include "antlr4-runtime.h"
#include "CParseTreeConverter.h"
#include "ASTNodes.h"

using namespace antlr_som;

namespace som {
	antlrcpp::Any CParseTreeConverter::visitClassdef(SOMParser::ClassdefContext* ctx)
	{
		nodeVector instMethods, classMethods;
		instMethods.resize(ctx->method().size());
		for (auto method : ctx->method()) {
			instMethods.emplace_back(visit(method).as<ASTNode*>());
		}
		classMethods.resize(ctx->classMethod().size());
		for (auto classMethod : ctx->classMethod()) {
			classMethods.emplace_back(visit(classMethod).as<ASTNode*>());
		}
		nodeVector classFields;
		if (ctx->classFields()) {
			classFields = std::move(visit(ctx->classFields()).as<nodeVector>());
		}
		return makeNode<Class>(
			ctx->Identifier()->toString(),
			visit(ctx->instanceFields()).as<nodeVector>(),
			instMethods,
			classFields,
			classMethods
			);
	}
	antlrcpp::Any CParseTreeConverter::visitString(SOMParser::StringContext* ctx)
	{
		return makeNode<LiteralString>(ctx->STString()->toString());
	}

	antlrcpp::Any CParseTreeConverter::visitVariable(SOMParser::VariableContext* ctx)
	{
		return makeNode<Variable>(visit(ctx->identifier()).as<std::string>());
	}

	antlrcpp::Any CParseTreeConverter::visitIdentifier(SOMParser::IdentifierContext* ctx)
	{
		// TODO: Handle primitive
		return ctx->Identifier()->toString();
	}

	antlrcpp::Any CParseTreeConverter::visitInstanceFields(SOMParser::InstanceFieldsContext* ctx)
	{
		return makeFieldsVector(ctx->variable());
	}

	antlrcpp::Any CParseTreeConverter::visitMethod(SOMParser::MethodContext* ctx)
	{
		return makeNode<Method>(
			std::unique_ptr<ASTNode>(visit(ctx->pattern()).as<ASTNode*>()),
			std::unique_ptr<ASTNode>(visit(ctx->methodBlock()).as<ASTNode*>())
		);
	}

	antlrcpp::Any CParseTreeConverter::visitMethodBlock(SOMParser::MethodBlockContext* ctx)
	{
		return ctx->blockContents() ?
			visit(ctx->blockContents()).as<ASTNode*>() :
			nullptr;
	}

	antlrcpp::Any CParseTreeConverter::visitClassFields(SOMParser::ClassFieldsContext* ctx)
	{
		return makeFieldsVector(ctx->variable());
	}

	antlrcpp::Any CParseTreeConverter::visitClassMethod(SOMParser::ClassMethodContext* ctx)
	{
		return visit(ctx->method());
	}

	antlrcpp::Any CParseTreeConverter::visitSuperclass(SOMParser::SuperclassContext* ctx)
	{
		return ctx->Identifier() ? ctx->Identifier()->toString() : std::string();
	}

	antlrcpp::Any CParseTreeConverter::visitPattern(SOMParser::PatternContext* ctx)
	{
		// Visits one of the unary/binary/keyword patterns
		return visitChildren(ctx).as<ASTNode*>();
	}

	antlrcpp::Any CParseTreeConverter::visitUnaryPattern(SOMParser::UnaryPatternContext* ctx)
	{
		return makeNode<UnaryPattern>(visit(ctx->unarySelector()->identifier()).as<std::string>());
	}

	antlrcpp::Any CParseTreeConverter::visitBinaryPattern(SOMParser::BinaryPatternContext* ctx)
	{
		ASTNode* rawSelector = visit(ctx->binarySelector()).as<ASTNode*>();
		ASTNode* rawArgument = visit(ctx->argument()).as<ASTNode*>();
		return new BinaryPattern(
			std::unique_ptr<ASTNode>(rawSelector),
			std::unique_ptr<ASTNode>(rawArgument)
		);
	}

	antlrcpp::Any CParseTreeConverter::visitBinarySelector(SOMParser::BinarySelectorContext* ctx)
	{
		BinaryOperation type;
		if (ctx->Or())
			type = BinaryOperation::Or;
		else if (ctx->Comma())
			type = BinaryOperation::Comma;
		else if (ctx->Minus())
			type = BinaryOperation::Minus;
		else if (ctx->Equal())
			type = BinaryOperation::Equal;
		else if (ctx->Not())
			type = BinaryOperation::Not;
		else if (ctx->And())
			type = BinaryOperation::And;
		else if (ctx->Star())
			type = BinaryOperation::Star;
		else if (ctx->Div())
			type = BinaryOperation::Div;
		else if (ctx->Mod())
			type = BinaryOperation::Mod;
		else if (ctx->Plus())
			type = BinaryOperation::Plus;
		else if (ctx->More())
			type = BinaryOperation::More;
		else if (ctx->Less())
			type = BinaryOperation::Less;
		else if (ctx->At())
			type = BinaryOperation::At;
		else if (ctx->Per())
			type = BinaryOperation::Per;
		else if (ctx->OperatorSequence())
			type = BinaryOperation::OperatorSequence;
		else
			type = BinaryOperation::Unknown;

		return makeNode<BinarySelector>(type);
	}

	antlrcpp::Any CParseTreeConverter::visitKeywordPattern(SOMParser::KeywordPatternContext* ctx)
	{
		nodeVector keywords, args;
		keywords.resize(ctx->keyword().size());
		args.resize(ctx->argument().size());
		// Handle if the number of kewords and arguments is not the same?
		for (int i = 0; i < ctx->keyword().size(); ++i) {
			ASTNode* keywordPtr = visit(ctx->keyword().at(i)).as<ASTNode*>();
			keywords.emplace_back(keywordPtr);
			ASTNode* argumentPtr = visit(ctx->argument().at(i)).as<ASTNode*>();
			args.emplace_back(argumentPtr);
		}
		return makeNode<KeywordPattern>(keywords, args);
	}

	antlrcpp::Any CParseTreeConverter::visitKeyword(SOMParser::KeywordContext* ctx)
	{
		return makeNode<Keyword>(ctx->Keyword()->toString());
	}

	antlrcpp::Any CParseTreeConverter::visitArgument(SOMParser::ArgumentContext* ctx)
	{
		return visit(ctx->variable());
	}

	antlrcpp::Any CParseTreeConverter::visitLocalDefs(SOMParser::LocalDefsContext* ctx)
	{
		 return makeFieldsVector(ctx->variable());
	}

	// Returns a vector of expressions to be evaluated in a block
	antlrcpp::Any CParseTreeConverter::visitBlockBody(SOMParser::BlockBodyContext* ctx)
	{
		nodeVector expressions;
		appendBlockExpression(ctx, expressions);
		return expressions;
	}

	void CParseTreeConverter::appendBlockExpression(SOMParser::BlockBodyContext* ctx, nodeVector& expressions)
	{
		if (ctx->result()) {
			expressions.emplace_back(visit(ctx->result()).as<ASTNode*>());
		}
		else if (ctx->expression()) {
			expressions.emplace_back(visit(ctx->expression()).as<ASTNode*>());
			if (ctx->blockBody()) {
				appendBlockExpression(ctx->blockBody(), expressions);
			}
		}
	}

	antlrcpp::Any CParseTreeConverter::visitNestedBlock(SOMParser::NestedBlockContext* ctx)
	{
		ASTNode* blockContents = nullptr;
		if (ctx->blockContents()) {
			blockContents = visit(ctx->blockContents()).as<ASTNode*>();
		}

		nodeVector blockPattern;
		if (ctx->blockPattern()) {
			blockPattern = std::move(visit(ctx->blockPattern()).as<nodeVector>());
		}
		
		return makeNode<NestedBlock>(
			blockPattern,
			std::unique_ptr<ASTNode>(blockContents)
			);
	}

	antlrcpp::Any CParseTreeConverter::visitBlockPattern(SOMParser::BlockPatternContext* ctx)
	{
		return visit(ctx->blockArguments());
	}

	antlrcpp::Any CParseTreeConverter::visitBlockArguments(SOMParser::BlockArgumentsContext* ctx)
	{
		nodeVector args;
		args.resize(ctx->argument().size());
		for (auto arg : ctx->argument()) {
			args.emplace_back(visit(arg).as<ASTNode*>());
		}
		return args;
	}

	antlrcpp::Any CParseTreeConverter::visitBlockContents(SOMParser::BlockContentsContext* ctx)
	{
		nodeVector localDefs;
		if (ctx->localDefs())
			localDefs = std::move(visit(ctx->localDefs()).as<nodeVector>());
		return makeNode<Block>(
			localDefs,
			visit(ctx->blockBody()).as<nodeVector>()
		);
	}

	nodeVector CParseTreeConverter::makeFieldsVector(const std::vector<SOMParser::VariableContext*>& variables)
	{
		nodeVector result;
		result.reserve(variables.size());
		for (auto var : variables) {
			ASTNode* varPtr = visit(var).as<ASTNode*>();
			result.emplace_back(varPtr);
		}
		return result;
	}

	// Literals
	antlrcpp::Any CParseTreeConverter::visitLiteralArray(SOMParser::LiteralArrayContext* ctx)
	{
		nodeVector arr;
		arr.resize(ctx->literal().size());
		for (auto lit : ctx->literal()) {
			ASTNode* literalPtr = visit(lit).as<ASTNode*>();
			arr.emplace_back(literalPtr);
		}
		return arr;
	}

	antlrcpp::Any CParseTreeConverter::visitLiteralString(SOMParser::LiteralStringContext* ctx)
	{
		return visitString(ctx->string());
	}

	antlrcpp::Any CParseTreeConverter::visitLiteralNumber(SOMParser::LiteralNumberContext* ctx)
	{
		bool isNegative = ctx->negativeDecimal() ? true : false;
		return ctx->negativeDecimal() ?
			createLiteralDecimal(ctx->negativeDecimal()->literalDecimal(), true) :
			createLiteralDecimal(ctx->literalDecimal(), false);
	}

	antlrcpp::Any CParseTreeConverter::createLiteralInteger(SOMParser::LiteralIntegerContext* ctx, bool isNegative)
	{
		int32_t sign = isNegative ? -1 : 1;
		return makeNode<LiteralInteger>((std::stoi(ctx->Integer()->toString())) * sign);
	}

	antlrcpp::Any CParseTreeConverter::createLiteralDouble(SOMParser::LiteralDoubleContext* ctx, bool isNegative)
	{
		double sign = isNegative ? -1 : 1;
		return makeNode<LiteralDouble>((std::stod(ctx->Double()->toString())) * sign);
	}

	antlrcpp::Any CParseTreeConverter::visitPrimary(SOMParser::PrimaryContext* ctx)
	{
		// Visits one of the: variable, nestedTerm, nestedBlock, literal
		return visitChildren(ctx).as<ASTNode*>();
	}

	antlrcpp::Any CParseTreeConverter::visitNestedTerm(SOMParser::NestedTermContext* ctx)
	{
		return makeNode<NestedTerm>(std::unique_ptr<ASTNode>(visit(ctx->expression()).as<ASTNode*>()));
	}

	antlrcpp::Any CParseTreeConverter::visitExpression(SOMParser::ExpressionContext* ctx)
	{
		// Visits one of the: assignation or evaluation
		return visitChildren(ctx).as<ASTNode*>();
	}

	antlrcpp::Any CParseTreeConverter::visitAssignation(SOMParser::AssignationContext* ctx)
	{
		return makeNode<Assignation>(
			visit(ctx->assignments()).as<nodeVector>(),
			std::unique_ptr<ASTNode>(visit(ctx->evaluation()).as<ASTNode*>())
		);
	}

	antlrcpp::Any CParseTreeConverter::visitAssignments(SOMParser::AssignmentsContext* ctx)
	{
		nodeVector assignments;
		assignments.resize(ctx->assignment().size());
		for (auto it : ctx->assignment()) {
			assignments.emplace_back(visit(it).as<ASTNode*>());
		}
		return assignments;
	}

	antlrcpp::Any CParseTreeConverter::visitAssignment(SOMParser::AssignmentContext* ctx)
	{
		return visit(ctx->variable()).as<ASTNode*>();
	}

	antlrcpp::Any CParseTreeConverter::visitEvaluation(SOMParser::EvaluationContext* ctx)
	{
		nodeVector messages;
		if (ctx->messages()) {
			messages = std::move(visit(ctx->messages()).as<nodeVector>());
		}
		return makeNode<Evaluation>(
			std::unique_ptr<ASTNode>(visit(ctx->primary()).as<ASTNode*>()),
			messages
		);
	}

	// Returns nodeVector of all unary, binary and keyword message sends
	antlrcpp::Any CParseTreeConverter::visitMessages(SOMParser::MessagesContext* ctx)
	{
		nodeVector messages;
		for (auto unaryMssg : ctx->unaryMessage()) {
			messages.emplace_back(visit(unaryMssg).as<ASTNode*>());
		}
		for (auto binaryMssg : ctx->binaryMessage()) {
			messages.emplace_back(visit(binaryMssg).as<ASTNode*>());
		}
		if (ctx->keywordMessage()) {
			messages.emplace_back(visit(ctx->keywordMessage()).as<ASTNode*>());
		}
		return messages;
	}

	antlrcpp::Any CParseTreeConverter::visitUnaryMessage(SOMParser::UnaryMessageContext* ctx)
	{
		return makeNode<UnaryMessage>(
			std::unique_ptr<ASTNode>(visit(ctx->unarySelector()).as<ASTNode*>())
		);
	}

	antlrcpp::Any CParseTreeConverter::visitUnarySelector(SOMParser::UnarySelectorContext* ctx)
	{
		return makeNode<UnarySelector>(visit(ctx->identifier()).as<std::string>());
	}

	antlrcpp::Any CParseTreeConverter::visitBinaryMessage(SOMParser::BinaryMessageContext* ctx)
	{
		return makeNode<BinaryMessage>(
			std::unique_ptr<ASTNode>(visit(ctx->binarySelector()).as<ASTNode*>()),
			std::unique_ptr<ASTNode>(visit(ctx->binaryOperand()).as<ASTNode*>())
		);
	}

	antlrcpp::Any CParseTreeConverter::visitBinaryOperand(SOMParser::BinaryOperandContext* ctx)
	{
		nodeVector mssgs;
		mssgs.resize(ctx->unaryMessage().size());
		for (auto mssg : ctx->unaryMessage()) {
			mssgs.emplace_back(visit(mssg).as<ASTNode*>());
		}
		return makeNode<BinaryOperand>(
			std::unique_ptr<ASTNode>(visit(ctx->primary()).as<ASTNode*>()),
			mssgs
			);
	}

	antlrcpp::Any CParseTreeConverter::visitKeywordMessage(SOMParser::KeywordMessageContext* ctx)
	{
		nodeVector keywords, formulas;
		assert(ctx->keyword().size() == ctx->formula().size());
		for (int i = 0; i < ctx->keyword().size(); ++i) {
			keywords.emplace_back(visit(ctx->keyword().at(i)).as<ASTNode*>());
			formulas.emplace_back(visit(ctx->formula().at(i)).as<ASTNode*>());
		}
		return makeNode<KeywordMessage>(keywords, formulas);
	}

	antlrcpp::Any CParseTreeConverter::visitFormula(SOMParser::FormulaContext* ctx)
	{
		nodeVector messages;
		for (auto mssg : ctx->binaryMessage()) {
			messages.emplace_back(visit(mssg).as<ASTNode*>());
		}
		return makeNode<Formula>(
			std::unique_ptr<ASTNode>(visit(ctx->binaryOperand()).as<ASTNode*>()),
			messages
		);
	}

	antlrcpp::Any CParseTreeConverter::visitResult(SOMParser::ResultContext* ctx)
	{
		return visit(ctx->expression()).as<ASTNode*>();
	}

	antlrcpp::Any CParseTreeConverter::createLiteralDecimal(SOMParser::LiteralDecimalContext* ctx, bool isNegative)
	{
		return ctx->literalInteger() ?
			createLiteralInteger(ctx->literalInteger(), isNegative) :
			createLiteralDouble(ctx->literalDouble(), isNegative);
	}

}