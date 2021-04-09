#include "Interpret.h"
#include "ExecutionStack.h"
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
        case OpCode::SetSlotOp:
            execute(static_cast<SetSlotIns*>(currentInstruction));
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
        StringValue* selectorVal = dynamic_cast<StringValue*>(m_program->getValue(ins->methodIdx));
        if (selectorVal) {
            if (receiver->isPrimitive(selectorVal->value)) {
                receiver->dispatch(selectorVal->value, m_executionStack);
            } else {
                // New stack frame

                // Get the address of the begining of the method and set pc to it

            }
        }
        m_pc.nextInstruction();
        interpret();
    }
    
    void CInterpret::execute(ReturnIns* ins)
    {
        m_pc.setAddress(m_executionStack.popFrame());
        interpret();
    }

    void CInterpret::execute(SetSlotIns* ins)
    {
        
    }

}