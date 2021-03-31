#include "Interpret.h"
#include "../bytecode/Bytecode.h"

namespace som {

    void CInterpret::interpret()
    {
        ByteIns* currentInstruction = (*m_pc.currentInstruction()).get();
        switch (currentInstruction->op)
        {
        case OpCode::LitOp:
            execute(static_cast<LitIns*>(currentInstruction));
            break;
        case OpCode::CallSlotOp:

            break;
        
        
        default:
            break;
        }
    }

    void CInterpret::execute(LitIns* ins)
    {
        Value* valPtr = m_program->getValue(ins->idx);
        m_executionStack.push(CObjectReference::fromValue(valPtr));
        m_pc.nextInstruction();
        interpret();
    }

}