#include <sstream>

#include "../ast/ASTNodes.h"
#include "BytecodeCompiler.h"
#include "BCClass.h"

namespace som {

    void CBytecodeCompiler::visit(Class* classNode)
    {
        size_t classNameIdx = m_program->registerConstant(classNode->m_identifier);
        // Handle the superclass - register new class and instantiate the object?
        
        // Instance fields - create slots
        for (const auto& insField : classNode->m_instanceFields) {
            // Get the name of the slot
            Variable* slotPtr = static_cast<Variable*>(insField.get());
            if (slotPtr) {
                visit(slotPtr);
            }
        }
        
        // Instance methods - similar thing to instance fields
        for (const auto& insMethod : classNode->m_instanceMethods) {
            Method* methodPtr = static_cast<Method*>(insMethod.get());
            if (!methodPtr) {
                // Handle the error
                continue;
            }
            visit(methodPtr);
        }
        
        m_program->registerClass(m_class);
    }

    void CBytecodeCompiler::visit(Method* method)
    {
        

        Block* methodBlock;
        methodBlock = static_cast<Block*>(method->m_methodBlock.get());
        int32_t nlocals = methodBlock->m_localDefs.size();

        UnaryPattern* unaryPtr;
        unaryPtr = dynamic_cast<UnaryPattern*>(method->m_pattern.get());
        if (unaryPtr) {
            int32_t patternIdx = m_program->registerConstant(unaryPtr->m_identifier);
            std::vector<ByteIns> code; // TODO: compile the instructions in the code
            int32_t methodIdx = m_program->registerMethod(patternIdx, 0, nlocals, code);
            m_class.registerSlot(methodIdx);
        }

        BinaryPattern* binPtr = dynamic_cast<BinaryPattern*>(method->m_pattern.get());
        if (binPtr) {
            // Argument?
            int32_t patternIdx = m_program->registerConstant(binPtr->m_identifier);
            std::vector<ByteIns> code; // TODO
            int32_t methodIdx = m_program->registerMethod(patternIdx, 1, nlocals, code);
            m_class.registerSlot(methodIdx);
        }

        KeywordPattern* keyPtr = dynamic_cast<KeywordPattern*>(method->m_pattern.get());
        if (keyPtr) {
            std::stringstream identifier;
            for (const auto& keyword : keyPtr->m_keywords) {
                Keyword* ptr = static_cast<Keyword*>(keyword.get());
                identifier << ptr->m_keyword;
            }
            int32_t patternIdx = m_program->registerConstant(identifier.str());
            std::vector<ByteIns> code; // TODO
            int32_t methodIdx = m_program->registerMethod(patternIdx, keyPtr->m_arguments.size(), nlocals, code);
            m_class.registerSlot(methodIdx);
        }
    }

    void CBytecodeCompiler::visit(UnaryPattern* unaryPattern)
    {
        // Intentionally empty
    }

    void CBytecodeCompiler::visit(BinaryPattern* binaryPattern)
    {
        size_t patternIdx = m_program->registerConstant(binaryPattern->m_identifier);
        m_class.registerSlot(patternIdx);

        // Register the argument
        Variable* argPtr = static_cast<Variable*>(binaryPattern->m_argument.get());
        if (argPtr) {
            visit(argPtr);
        }
    }
    
    void CBytecodeCompiler::visit(LiteralInteger* litInteger)
    {

    }

    void CBytecodeCompiler::visit(LiteralDouble* litDouble)
    {

    }

    void CBytecodeCompiler::visit(LiteralString* litString)
    {

    }

}