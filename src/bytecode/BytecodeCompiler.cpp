#include <sstream>
#include <cassert>
#include <any>

#include "../ast/ASTNodes.h"
#include "BytecodeCompiler.h"
#include "BCClass.h"

namespace som {

    std::any CBytecodeCompiler::visit(Class* classNode)
    {
        size_t classNameIdx = m_program->registerConstant(classNode->m_identifier);
        std::vector<int32_t> slots;
        // Handle the superclass - register new class and instantiate the object?
        
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
        m_program->registerClass(classNameIdx, slots);
        return std::make_any<bool>(true);
    }

    std::any CBytecodeCompiler::visit(Method* method)
    {
        if (method->m_primitive) {
            int32_t patternIdx = std::any_cast<int32_t>(visit(method->m_pattern.get()));
            return std::make_any<int32_t>(m_program->registerMethod(patternIdx));
        }
        
        Block* methodBlock = dynamic_cast<Block*>(method->m_methodBlock.get());
        int32_t nlocals = methodBlock ? methodBlock->m_localDefs->size() : 0;
        insVector* instructions = std::any_cast<insVector*>(visit(methodBlock));
        instructions->emplace_back(new ReturnIns());

        UnaryPattern* unaryPtr = dynamic_cast<UnaryPattern*>(method->m_pattern.get());
        if (unaryPtr) {
            int32_t patternIdx = std::any_cast<int32_t>(visit(unaryPtr));
            int32_t methodIdx = m_program->registerMethod(patternIdx, 0, nlocals, instructions);
            if (unaryPtr->m_identifier == "run") {
                m_program->setEntryPoint(methodIdx);
            }
            return std::make_any<int32_t>(methodIdx);
        }

        BinaryPattern* binPtr = dynamic_cast<BinaryPattern*>(method->m_pattern.get());
        if (binPtr) {
            int32_t patternIdx = std::any_cast<int32_t>(visit(binPtr));
            return std::make_any<int32_t>(m_program->registerMethod(patternIdx, 1, nlocals, instructions));
        }

        KeywordPattern* keyPtr = dynamic_cast<KeywordPattern*>(method->m_pattern.get());
        if (keyPtr) {
            int32_t patternIdx = std::any_cast<int32_t>(visit(keyPtr));
            return std::make_any<int32_t>(m_program->registerMethod(patternIdx, keyPtr->m_keywords->size(), nlocals, instructions));
        }

        // Unreachable
        return std::any();
    }

    std::any CBytecodeCompiler::visit(UnaryPattern* unaryPattern)
    {
        return std::make_any<int32_t>(m_program->registerConstant(unaryPattern->m_identifier));
    }

    std::any CBytecodeCompiler::visit(BinaryPattern* binaryPattern)
    {
        return std::make_any<int32_t>(m_program->registerConstant(binaryPattern->m_identifier));
    }

    std::any CBytecodeCompiler::visit(KeywordPattern* keywordPattern)
    {
        std::stringstream selector;
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

        // Handle error - no such slot
        return std::any();
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
            result->emplace_back(new SetSlotIns(m_program->indexOf(varPtr->m_identifier)));
        }
       
        return std::make_any<insVector*>(result);
    }

    std::any CBytecodeCompiler::visit(Evaluation* evaluation)
    {
        insVector* result = new insVector();
        // nodePtr m_primary - variable, nested term, nested block, literal
        Variable* varPtr = dynamic_cast<Variable*>(evaluation->m_primary.get());
        if (varPtr) {
            // Get the slot
            int32_t varIdx = m_program->indexOf(varPtr->m_identifier);
            if (varIdx < 0) {
                varIdx = m_program->registerConstant(varPtr->m_identifier);
            } 
            GetSlotIns* newIns = new GetSlotIns(varIdx);
            result->emplace_back(newIns);
        } else {
            insVector* primary = std::any_cast<insVector*>(visit(evaluation->m_primary.get()));
            appendInstructions(result, primary);
        }

        // nodeVectorPtr m_messages -- add calls to slots in correct order
        if (evaluation->m_messages) {
            for (const auto& mssg : *(evaluation->m_messages)) {
                UnaryMessage* unMssg = dynamic_cast<UnaryMessage*>(mssg.get());
                if (unMssg) {
                    result->emplace_back(std::any_cast<SendIns*>(visit(unMssg)));
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
        result->emplace_back(new LitIns(m_program->indexOf(litString->m_value)));
        return std::make_any<insVector*>(result);
    }


    void CBytecodeCompiler::appendInstructions(insVector* first, insVector* second) const
    {
        for (auto& ins : *second) {
            first->emplace_back(std::move(ins));
        }
        delete second;
    }

}