#include <sstream>
#include <cassert>
#include <any>

#include "../ast/ASTNodes.h"
#include "BytecodeCompiler.h"
#include "BCClass.h"

namespace som {

    std::any CBytecodeCompiler::visit(Class* classNode)
    {
        m_nlReturnLvl = 0;
        size_t classNameIdx = m_program->registerConstant(classNode->m_identifier);
        std::vector<int32_t> slots;

        int32_t superclass = -1;
        // if (classNode->)        
        
        // Instance fields - create slots
        for (const auto& insField : *(classNode->m_instanceFields)) {
            Variable* variablePtr = dynamic_cast<Variable*>(insField.get());
            if (variablePtr) {
                int32_t identifierIdx = std::any_cast<int32_t>(visit(variablePtr));
                slots.push_back(identifierIdx);
            }
        }
        
        // Instance methods - similar thing to instance fields
        for (const auto& insMethod : *(classNode->m_instanceMethods)) {
            slots.push_back(std::any_cast<int32_t>(visit(insMethod.get())));
        }
        m_program->registerClass(classNameIdx, superclass, slots);
        return std::make_any<bool>(true);
    }

    std::any CBytecodeCompiler::visit(Method* method)
    {       
        m_scopes.newScope();
        if (method->m_primitive) {
            return std::make_any<int32_t>(primitiveMethod(method));
        }
        
        Block* methodBlock = dynamic_cast<Block*>(method->m_methodBlock.get());
        int32_t nlocals = methodBlock ? methodBlock->m_localDefs->size() : 0;
        int32_t arity, patternIdx;
        bool isEntryPoint = false;
        
        if (methodBlock)
            m_scopes.registerLocals(methodBlock->m_localDefs.get());


        UnaryPattern* unaryPtr = dynamic_cast<UnaryPattern*>(method->m_pattern.get());
        if (unaryPtr) {
            patternIdx = std::any_cast<int32_t>(visit(unaryPtr));
            arity = 0;
            if (unaryPtr->m_identifier == "run") {
                isEntryPoint = true;
            }
        }

        BinaryPattern* binPtr = dynamic_cast<BinaryPattern*>(method->m_pattern.get());
        if (binPtr) {
            patternIdx = std::any_cast<int32_t>(visit(binPtr));
            arity = 1;
        }

        KeywordPattern* keyPtr = dynamic_cast<KeywordPattern*>(method->m_pattern.get());
        if (keyPtr) {
            patternIdx = std::any_cast<int32_t>(visit(keyPtr));
            arity = keyPtr->m_arguments->size();
        }

        insVector* instructions = std::any_cast<insVector*>(visit(methodBlock));
        instructions->emplace_back(new ReturnIns());

        int32_t methodIdx = m_program->registerMethod(patternIdx, arity, nlocals, instructions);
        if (isEntryPoint)
            m_program->setEntryPoint(methodIdx);

        m_scopes.popScope();
        return std::make_any<int32_t>(methodIdx);
    }

    int32_t CBytecodeCompiler::primitiveMethod(Method* method)
    {
        int32_t patternIdx = std::any_cast<int32_t>(visit(method->m_pattern.get()));
        int32_t arity = 0;
        if (dynamic_cast<BinaryPattern*>(method->m_pattern.get())) {
            arity = 1;
        } else if (dynamic_cast<KeywordPattern*>(method->m_pattern.get())) {
            arity = dynamic_cast<KeywordPattern*>(method->m_pattern.get())->m_keywords->size();
        }
        m_scopes.popScope();
        return m_program->registerMethod(patternIdx, arity);
    }

    std::any CBytecodeCompiler::visit(UnaryPattern* unaryPattern)
    {
        return std::make_any<int32_t>(m_program->registerConstant(unaryPattern->m_identifier));
    }

    std::any CBytecodeCompiler::visit(BinaryPattern* binaryPattern)
    {
        // Register arguments
        m_scopes.addArgument(dynamic_cast<Variable*>(binaryPattern->m_argument.get())->m_identifier);
        return std::make_any<int32_t>(m_program->registerConstant(binaryPattern->m_identifier));
    }

    std::any CBytecodeCompiler::visit(KeywordPattern* keywordPattern)
    {
        std::stringstream selector;
        for (const auto& arg : *keywordPattern->m_arguments) {
            m_scopes.addArgument(dynamic_cast<Variable*>(arg.get())->m_identifier);
        }
        for (const auto& keyword : *keywordPattern->m_keywords) {
            selector << std::any_cast<std::string>(visit(keyword.get()));
        }
        return std::make_any<int32_t>(m_program->registerConstant(selector.str()));
    }

    std::any CBytecodeCompiler::visit(Keyword* keyword) 
    {
        return std::make_any<std::string>(keyword->m_keyword);
    }

    std::any CBytecodeCompiler::visit(Variable* variable)
    {
        return std::make_any<int32_t>(m_program->indexOf(variable->m_identifier));
    }

    ByteIns* CBytecodeCompiler::resolveVariable(ASTNode* node)
    {
        Variable* varPtr = dynamic_cast<Variable*>(node);
        if (varPtr) {
            if (varPtr->m_identifier == "self") {
                return new GetSelfIns();
            }
            int32_t localIdx = m_scopes.localIdx(varPtr->m_identifier);
            if (localIdx >= 0) {
                return new GetLocalIns(localIdx);
            }
            int32_t argIdx = m_scopes.argIdx(varPtr->m_identifier);
            if (argIdx >= 0) {
                return new GetArgIns(argIdx);
            }
            return new GetSlotIns(m_program->indexOf(varPtr->m_identifier));
        }

        return nullptr;
    }

    std::any CBytecodeCompiler::visit(Block* block)
    {
        // Register local definitions
        for (const auto& localDef : *(block->m_localDefs)) {
            Variable* localDefPtr = static_cast<Variable*>(localDef.get());
            int32_t identifierIdx = std::any_cast<int32_t>(visit(localDefPtr));
            
        }

        insVector* blockCode = new insVector();
        // Compile expressions
        for (const auto& exp : *(block->m_expressions)) {
           insVector* compiledExpression = std::any_cast<insVector*>(visit(exp.get()));
           appendInstructions(blockCode, compiledExpression);
        }
        return std::make_any<insVector*>(blockCode);
    }

    std::any CBytecodeCompiler::visit(UnaryMessage* unaryMessage)
    {
        // Selector -> index of string in constants pool
        UnarySelector* unSelector = static_cast<UnarySelector*>(unaryMessage->m_unarySelector.get());
        int32_t selectorIdx = m_program->indexOf(unSelector->m_identifier);
        
        SendIns* newCall = new SendIns(selectorIdx, 0);
        return newCall;

    }

    std::any CBytecodeCompiler::visit(BinaryMessage* binaryMessage)
    {
        int32_t selectorIdx = m_program->indexOf(binaryMessage->m_identifier);
        insVector* result = new insVector();
        appendInstructions(result, std::any_cast<insVector*>(visit(binaryMessage->m_operand.get())));
        result->emplace_back(new SendIns(selectorIdx, 1));
        return result;
    }

    std::any CBytecodeCompiler::visit(BinaryOperand* binaryOperand)
    {
        insVector* result = new insVector();
        // Push the value on the stack
        appendInstructions(result, std::any_cast<insVector*>(resolvePrimary(binaryOperand->m_primary.get())));
        for (const auto& mssg : *binaryOperand->m_unaryMessages) {
            result->emplace_back(std::any_cast<SendIns*>(visit(mssg.get())));
        }
        return result;
    }

    std::any CBytecodeCompiler::visit(KeywordMessage* keywordMessage)
    {
        insVector* result = new insVector();
        for (const auto& formula : *keywordMessage->m_formulas) {
            appendInstructions(result, std::any_cast<insVector*>(visit(formula.get())));
        }

        std::stringstream selector;
        for (const auto& key : *keywordMessage->m_keywords) {
            selector << std::any_cast<std::string>(visit(key.get()));
        }
        result->emplace_back(new SendIns(m_program->indexOf(selector.str()), keywordMessage->m_keywords->size()));

        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(Formula* formula)
    {
        insVector* result = new insVector();
        if (formula->m_argument) {
            appendInstructions(result, resolvePrimary(formula->m_argument.get())); // ???
        }
        for (const auto& mssg : *formula->m_binaryMessage) {
            appendInstructions(result, std::any_cast<insVector*>(visit(mssg.get())));
        }
        return result;
    }

    std::any CBytecodeCompiler::visit(Assignation* assignation)
    {
        // nodeVectorPtr m_variables
        // nodePtr m_value
        insVector* result = new insVector();
        // Retrieve value, push to stack
        for (const auto& var : *assignation->m_variables) {
            // Push the new value to the stack
            appendInstructions(result, std::any_cast<insVector*>(visit(assignation->m_value.get())));
            Variable* varPtr = dynamic_cast<Variable*>(var.get());
            // Set the variable to the value on top of the stack
            if (m_scopes.localIdx(varPtr->m_identifier) >= 0) {
                result->emplace_back(new SetLocalIns(m_scopes.localIdx(varPtr->m_identifier)));
            } else {
                result->emplace_back(new SetSlotIns(m_program->indexOf(varPtr->m_identifier)));
            }   
        }
       
        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(Evaluation* evaluation)
    {
        insVector* result = new insVector();
        appendInstructions(result, std::any_cast<insVector*>(resolvePrimary(evaluation->m_primary.get())));

        // nodeVectorPtr m_messages -- add calls to slots in correct order
        if (evaluation->m_messages) {
            for (const auto& mssg : *(evaluation->m_messages)) {
                UnaryMessage* unMssg = dynamic_cast<UnaryMessage*>(mssg.get());
                if (unMssg) {
                    result->emplace_back(std::any_cast<SendIns*>(visit(unMssg)));
                }

                BinaryMessage* binMssg = dynamic_cast<BinaryMessage*>(mssg.get());
                if (binMssg) {
                    appendInstructions(result, std::any_cast<insVector*>(visit(binMssg)));
                }

                KeywordMessage* keyMssg = dynamic_cast<KeywordMessage*>(mssg.get());
                if (keyMssg) {
                    appendInstructions(result, std::any_cast<insVector*>(visit(keyMssg)));
                }
            }
        }
        return std::make_any<insVector*>(result);
    }
    
    
    // Literals
    std::any CBytecodeCompiler::visit(LiteralInteger* litInteger)
    {
        insVector* result = new insVector();
        result->emplace_back(new LitIns(m_program->indexOf(litInteger->m_value)));
        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(LiteralDouble* litDouble)
    {
        insVector* result = new insVector();
        result->emplace_back(new LitIns(m_program->indexOf(litDouble->m_value)));
        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(LiteralString* litString)
    {
        insVector* result = new insVector();
        trimString(litString->m_value);
        result->emplace_back(new LitIns(m_program->indexOf(litString->m_value)));
        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(LiteralArray* litArray)
    {
        insVector* result = new insVector();
        insVector* elements = new insVector();

        for (const auto& elem : *litArray->m_literals) {
            appendInstructions(elements, std::any_cast<insVector*>(visit(elem.get())));
        }

        result->emplace_back(new LitIns(m_program->registerArray(elements)));
        return std::make_any<insVector*>(result);
    }

    void CBytecodeCompiler::trimString(std::string& val)
    {
        if (val.front() == '\'') {
            val.erase(0, 1);
        }
        if (val.back() == '\'') {
            val.erase(val.size() - 1, 1);
        }
    }

    std::any CBytecodeCompiler::visit(NestedTerm* nestedTerm)
    {
        return visit(nestedTerm->m_expression.get());
    }

    std::any CBytecodeCompiler::visit(NestedBlock* nestedBlock)
    {
        m_scopes.newScope();
        m_nlReturnLvl++;
        int32_t nargs;
        if (nestedBlock->m_arguments) {
            for (const auto& arg : *nestedBlock->m_arguments) {
                m_scopes.addArgument(dynamic_cast<Variable*>(arg.get())->m_identifier);
            }
            nargs = nestedBlock->m_arguments->size();
        } else {
            nargs = 0;
        }
        insVector* compiledBlock = std::any_cast<insVector*>(visit(nestedBlock->m_block.get()));
        compiledBlock->emplace_back(new ReturnNLIns(m_nlReturnLvl + 1));
        int32_t blockValIdx = m_program->registerBlock(nargs, compiledBlock);

        insVector* result = new insVector();
        result->emplace_back(new BlockIns(blockValIdx));
        m_scopes.popScope();
        return result;
    }


    void CBytecodeCompiler::appendInstructions(insVector* first, insVector* second) const
    {
        for (auto& ins : *second) {
            first->emplace_back(std::move(ins));
        }
        delete second;
    }

    insVector* CBytecodeCompiler::resolvePrimary(ASTNode* node)
    {
        // Might need some more finagling
        insVector* result = new insVector();
        Variable* varPtr = dynamic_cast<Variable*>(node);
        if (varPtr) {
            result->emplace_back(resolveVariable(varPtr));
            return result;
        } else {
            delete result;
            return std::any_cast<insVector*>(visit(node)); // Literal, nested term, nested block
        }
    }

}