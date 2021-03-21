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
        UnaryPattern* unaryPtr;
        unaryPtr = static_cast<UnaryPattern*>(method->m_pattern.get());

        Block* methodBlock;
        methodBlock = static_cast<Block*>(method->m_methodBlock.get());
        if (unaryPtr) {
            int32_t patternIdx = m_program->registerConstant(unaryPtr->m_identifier);
            std::vector<ByteIns> code; // TODO: compile the instructions in the code
            int32_t nlocals = methodBlock->m_localDefs.size();
            int32_t methodIdx = m_program->registerMethod(patternIdx, 0, nlocals, code);
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