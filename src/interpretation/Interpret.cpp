#include <iostream>
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
            case OpCode::GetSlotOp:
                execute(static_cast<GetSlotIns*>(currentInstruction));
                break;
            case OpCode::SetSlotOp:
                execute(static_cast<SetSlotIns*>(currentInstruction));
                break;
            case OpCode::GetLocalOp:
                execute(static_cast<GetLocalIns*>(currentInstruction));
                break;
            case OpCode::SetLocalOp:
                execute(static_cast<SetLocalIns*>(currentInstruction));
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
            case OpCode::ReturnNLOp:
                execute(static_cast<ReturnNLIns*>(currentInstruction));
                return; // Jump out of the nested interpret() call
        
        
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

    std::shared_ptr<VMObject>& CInterpret::objFromValue(Value* val)
    {
        std::shared_ptr<VMClass> clazz;
        switch (val->tag)
        {
        case ValueTag::StringVal:
            clazz = m_globalCtx.getClass("String");
            return m_heap.newObject(clazz, VMValue(dynamic_cast<StringValue*>(val)->value));
        case ValueTag::IntVal:
            clazz = m_globalCtx.getClass("Integer");
            return m_heap.newObject(clazz, VMValue(dynamic_cast<IntValue*>(val)->value));
        case ValueTag::DoubleVal:
            clazz = m_globalCtx.getClass("Double");
            return m_heap.newObject(clazz, VMValue(dynamic_cast<DoubleValue*>(val)->value));
        case ValueTag::ArrayVal:
            return newArrayObj(dynamic_cast<ArrayValue*>(val));

        default:
            std::cout << "Unknown value tag encountered.\n";
            exit(1);
        }
    }

    std::shared_ptr<VMObject>& CInterpret::newArrayObj(ArrayValue* val)
    {
        std::vector<std::shared_ptr<VMObject>> elements;
        CodeAddress currentIns = m_pc.currentInstruction();
        for (const auto& elem : *val->values) {
            LitIns* litIns = dynamic_cast<LitIns*>(elem.get());
            if (litIns)
                execute(litIns);
            GetSlotIns* getSlotIns = dynamic_cast<GetSlotIns*>(elem.get());
            if (getSlotIns)
                execute(getSlotIns);
            GetArgIns* getArgIns = dynamic_cast<GetArgIns*>(elem.get());
            if (getArgIns)
                execute(getArgIns);
            elements.push_back(m_executionStack.pop());
        }
        auto& clazz = m_globalCtx.getClass("Array");
        m_pc.setAddress(currentIns);
        return m_heap.newObject(clazz, VMValue(std::move(elements)));
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
            receiver->getClass()->dispatchPrimitive(selector, m_pc.nextInstruction(), ins->arity, this);
            simpleReturn();
        } else {
            CodeAddress newMethod = receiver->getClass()->getMethodAddr(selector, m_globalCtx);
            m_executionStack.pushFrame(m_pc.nextInstruction(), ins->arity + 1, receiver->getClass()->className() == "Block");
            m_pc.setAddress(newMethod);
            m_executionStack.topFrame().setInitialAddress(m_pc.currentInstruction());
        }
    }
    
    void CInterpret::execute(ReturnIns* ins)
    {
        simpleReturn();
    }

    void CInterpret::execute(SetSlotIns* ins)
    {
        auto& self = m_executionStack.getSelf();
        self->setField(m_program->getStringValue(ins->slotIdx), m_executionStack.pop());
        m_pc.nextInstruction();
    }

    void CInterpret::execute(GetSlotIns* ins)
    {
        auto& self = m_executionStack.getSelf();
        auto slot = resolveIdentifier(m_program->getStringValue(ins->slotIdx), self);
        m_executionStack.push(slot);
        m_pc.nextInstruction();
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
        auto& newObj = blockClass->newObject(m_heap, m_globalCtx, VMValue(blockAddr, m_executionStack.topFramePtr()));
        m_executionStack.push(newObj);
        m_pc.nextInstruction();
    }

    void CInterpret::execute(ReturnNLIns* ins)
    {
        if (ins->lvl == 1) {
            m_localRetFromBlock = true;
            return;
        }
        auto& blockHomeContext = m_executionStack.getSelf()->getValue().asBlockContext().homeCtx;
        do {
            simpleReturn();
        } while (m_executionStack.topFramePtr() != blockHomeContext && !m_executionStack.empty());
        if (m_executionStack.empty()) {
            std::cout << "Block home context terminated before block evaluation." << std::endl;
            exit(1);
        }
    }

    void CInterpret::execute(GetLocalIns* ins)
    {
        m_executionStack.push(m_executionStack.getLocal(ins->idx));
        m_pc.nextInstruction();
    }

    void CInterpret::execute(SetLocalIns* ins)
    {
        m_executionStack.setLocal(ins->idx, m_executionStack.top());
        m_executionStack.pop();
        m_pc.nextInstruction();
    }

    void CInterpret::simpleReturn()
    {
        if (m_executionStack.size() <= 1) {
            return; // Non-local return out of the program has been performed
        }
        auto oldFrame = m_executionStack.popFrame();
        if (m_localRetFromBlock) {
            m_pc.setAddress(oldFrame.localReturnAddress());
            m_localRetFromBlock = false;
        } else {
            m_pc.setAddress(oldFrame.returnAddress());
        }
        m_executionStack.push(oldFrame.top());        
    }

    std::shared_ptr<VMObject> CInterpret::resolveIdentifier(const std::string& identifier, std::shared_ptr<VMObject>& self)
    {
        if (self->getClass()->hasField(identifier)) {
            return self->getField(identifier);
        }
        if (m_globalCtx.getObject(identifier))
            return m_globalCtx.getObject(identifier);
        
        return m_globalCtx.getClass(identifier);
        
    }

}