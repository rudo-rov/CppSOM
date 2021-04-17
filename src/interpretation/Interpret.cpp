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
        case ValueTag::DoubleVal:
            clazz = m_globalCtx.getClass("Double");
            return std::make_shared<VMObject>(clazz, VMValue(dynamic_cast<DoubleValue*>(val)->value));
        
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
        // ARITY!!! receiver is not always on the top of the stack
        auto receiver = m_executionStack.top();
        std::string selector = m_program->getStringValue(ins->methodIdx);
        if (receiver->getClass()->isPrimitive(selector)) {
            // Dispatch the primitive method call
            receiver->getClass()->dispatchPrimitive(selector, m_executionStack, m_globalCtx, m_pc.nextInstruction(), ins->arity);
            m_pc.setAddress(m_executionStack.topFrame().returnAddress());
            auto retValue = m_executionStack.pop();
            m_executionStack.popFrame();
            m_executionStack.push(retValue);
        }
        interpret();
    }
    
    void CInterpret::execute(ReturnIns* ins)
    {
        auto oldFrame = m_executionStack.popFrame();
        m_pc.setAddress(oldFrame.returnAddress());
        // Push the return value to the current frame
        m_executionStack.push(oldFrame.top());
        interpret();
    }

    void CInterpret::execute(SetSlotIns* ins)
    {
        
    }

    void CInterpret::execute(GetArgIns* ins)
    {
        m_executionStack.push(m_executionStack.getArgument(ins->idx));
    }

}