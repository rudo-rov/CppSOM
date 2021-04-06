#include "Interpret.h"
#include "../bytecode/Bytecode.h"

namespace som {

    void CInterpret::interpret()
    {
        if (m_pc.shouldExit()) return;
        ByteIns* currentInstruction = (*m_pc.currentInstruction()).get();
        switch (currentInstruction->op)
        {
        case OpCode::LitOp:
            execute(static_cast<LitIns*>(currentInstruction));
            break;
        case OpCode::SendOp:
            execute(static_cast<SendIns*>(currentInstruction));
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

    void CInterpret::execute(SendIns* ins)
    {
        auto receiver = m_executionStack.pop();
        m_executionStack.pushFrame(++m_pc.currentInstruction());
        // m_pc.setAddress(receiver->method(ins->methodIdx));
        interpret();
    }
    
    void CInterpret::execute(ReturnIns* ins)
    {
        m_pc.setAddress(m_executionStack.popFrame());
        interpret();
    }

}