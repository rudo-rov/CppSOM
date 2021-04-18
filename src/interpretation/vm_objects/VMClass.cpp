#include "VMClass.h"

namespace som {

    void VMClass::addSlot(const Program* program, int32_t slotIdx)
    {
        switch (program->getValue(slotIdx)->tag)
        {
        case ValueTag::PrimitiveVal:
            addPrimitive(program, program->getValue(slotIdx));
            break;
        case ValueTag::MethodVal:
            addMethod(program, program->getValue(slotIdx));
        
        default:
            break;
        }
    }

    bool VMClass::isPrimitive(const std::string& selector) const
    {
        for (const auto& prim : m_primitives) {
            if (prim == selector)
                return true;
        }
        return false;
    }

    void VMClass::addPrimitive(const Program* program, const Value* valPtr)
    {
        const PrimitiveValue* primPtr = dynamic_cast<const PrimitiveValue*>(valPtr);
        if (primPtr) {
            StringValue* selectorVal = dynamic_cast<StringValue*>(program->getValue(primPtr->name));
            if (selectorVal)
                m_primitives.push_back(selectorVal->value);
        }
    }

    void VMClass::addMethod(const Program* program, const Value* valPtr)
    {
        const MethodValue* methodPtr = dynamic_cast<const MethodValue*>(valPtr);
        if (methodPtr) {
            
        }
    }

    std::shared_ptr<VMObject> VMClass::newObject(CHeap& heap)
    {
        return heap.newObject(this);
    }

}