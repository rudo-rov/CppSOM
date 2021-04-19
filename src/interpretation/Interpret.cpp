#include "Interpret.h"
#include "ExecutionStack.h"
#include "../bytecode/Bytecode.h"
#include "vm_objects/VMValue.h"

namespace som {

    void CInterpret::interpret()
    {
        while (!shouldExit()) {
            ByteIns* currentInstruction = (*m_pc.currentInstruction()).get();
            // currentInstruction->print();
            switch (currentInstruction->op)
            {
            case OpCode::LitOp:
                execute(static_cast<LitIns*>(currentInstruction));
                break;
            case OpCode::BlockOp:
                execute(static_cast<BlockIns*>(currentInstruction));
                break;
            case OpCode::SendOp:
                execute(static_cast<SendIns*>(currentInstruction));
                break;
            case OpCode::SetSlotOp:
                execute(static_cast<SetSlotIns*>(currentInstruction));
                break;
            case OpCode::ReturnOp:
                execute(static_cast<ReturnIns*>(currentInstruction));
                break;
            case OpCode::GetSelfOp:
                execute(static_cast<GetSelfIns*>(currentInstruction));
                break;
            case OpCode::GetArgOp:
                execute(static_cast<GetArgIns*>(currentInstruction));
                break;
        
        
            default:
                break;
            }
        }
        
    }

    bool CInterpret::shouldExit() const
    {
        if (m_executionStack.size() > 2) {
            return false;
        }
        return m_pc.shouldExit();
    }

    void CInterpret::initialize()
    {
        m_globalCtx.initialize(m_program.get());
        m_executionStack.pushFrame(m_pc.getProgramEnd());
        auto rootObject = m_globalCtx.getRunClass()->newObject(m_heap, m_globalCtx);
        m_executionStack.push(rootObject);
        m_executionStack.pushFrame(m_pc.getProgramEnd(), 1); // The root object as receiver of the run message

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
    }

    void CInterpret::execute(SendIns* ins)
    {
        auto receiver = m_executionStack.fromTop(ins->arity);
        std::string selector = m_program->getStringValue(ins->methodIdx);
        if (receiver->getClass()->isPrimitive(selector)) {
            // Dispatch the primitive method call
            receiver->getClass()->dispatchPrimitive(selector, m_executionStack, m_globalCtx, m_pc.nextInstruction(), ins->arity);
            m_pc.setAddress(m_executionStack.topFrame().returnAddress());
            auto retValue = m_executionStack.pop();
            m_executionStack.popFrame();
            m_executionStack.push(retValue);
        } else {
            CodeAddress newMethod = receiver->getClass()->getMethodAddr(selector);
            m_executionStack.pushFrame(m_pc.nextInstruction(), ins->arity + 1);
            m_pc.setAddress(newMethod);
        }
    }
    
    void CInterpret::execute(ReturnIns* ins)
    {
        auto oldFrame = m_executionStack.popFrame();
        m_pc.setAddress(oldFrame.returnAddress());
        // Push the return value to the current frame
        m_executionStack.push(oldFrame.top());
    }

    void CInterpret::execute(SetSlotIns* ins)
    {
        
    }

    void CInterpret::execute(GetArgIns* ins)
    {
        m_executionStack.push(m_executionStack.getArgument(ins->idx));
        m_pc.nextInstruction();
    }

    void CInterpret::execute(GetSelfIns* ins)
    {
        m_executionStack.push(m_executionStack.getSelf());
        m_pc.nextInstruction();
    }

    void CInterpret::execute(BlockIns* ins)
    {
        auto& blockClass = m_globalCtx.getClass("Block");
        auto blockAddr = dynamic_cast<BlockValue*>(m_program->getValue(ins->idx))->code->begin();
        auto& newObj = blockClass->newObject(m_heap, m_globalCtx, VMValue(blockAddr));
        m_executionStack.push(newObj);
        m_pc.nextInstruction();
    }

}