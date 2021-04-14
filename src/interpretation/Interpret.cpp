#include "Interpret.h"
#include "ExecutionStack.h"
#include "../bytecode/Bytecode.h"
#include "vm_objects/VMValue.h"

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

    void CInterpret::initialize()
    {
        m_globalCtx.initialize(m_program.get());
    }

    std::shared_ptr<VMObject> CInterpret::objFromValue(Value* val)
    {
        std::shared_ptr<VMClass> clazz;
        switch (val->tag)
        {
        case ValueTag::StringVal:
            clazz = m_globalCtx.getClass("String");
            return std::make_shared<VMObject>(clazz, VMValue(dynamic_cast<StringValue*>(val)->value));
        case ValueTag::IntVal:
            clazz = m_globalCtx.getClass("Integer");
            return std::make_shared<VMObject>(clazz, VMValue(dynamic_cast<IntValue*>(val)->value));
        
        default:
            return std::make_shared<VMObject>(); // Should not be reachable
        }
    }

    void CInterpret::execute(LitIns* ins)
    {
        Value* valPtr = m_program->getValue(ins->idx);
        m_executionStack.push(objFromValue(valPtr));
        m_pc.nextInstruction();
        interpret();
    }

    void CInterpret::execute(SendIns* ins)
    {
        auto receiver = m_executionStack.top();
        std::string selector = m_program->getStringValue(ins->methodIdx);
        if (receiver->getClass()->isPrimitive(selector)) {
            // Dispatch the primitive method call
            receiver->getClass()->dispatchPrimitive(selector, m_executionStack);
        }
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