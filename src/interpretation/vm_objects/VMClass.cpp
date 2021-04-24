#include <iostream>
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
            break;
        case ValueTag::StringVal:
            addInstanceField(program->getStringValue(slotIdx));
        
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

    CodeAddress VMClass::getMethodAddr(const std::string& selector, CGlobalContext& globalCtx)
    {
        if (m_methods.find(selector) != m_methods.end())
            return m_methods[selector];
        if (m_identifier != "Object")
            return superclass(globalCtx)->getMethodAddr(selector, globalCtx);
        else {
            std::cout << "Method not found: " << selector << std::endl;
            exit(0);
        }
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
            m_methods[program->getStringValue(methodPtr->name)] = methodPtr->code->begin();
        }
    }

    void VMClass::addInstanceField(const std::string& identifier)
    {
        m_instanceFields.push_back(identifier);
    }

    std::shared_ptr<VMObject> VMClass::newObject(CHeap& heap, CGlobalContext& globalCtx)
    {
        return heap.newObject(globalCtx.getClass(m_identifier), globalCtx);
    }

    std::shared_ptr<VMObject> VMClass::newObject(CHeap& heap, CGlobalContext& globalCtx, VMValue val)
    {
        return heap.newObject(globalCtx.getClass(m_identifier), val);
    }

    bool VMClass::hasField(const std::string& identifier) const
    {
        for (const auto& field : m_instanceFields) {
            if (field == identifier)
                return true;
        }
        return false;
    }

    std::shared_ptr<VMClass>& VMClass::superclass(CGlobalContext& globalCtx)
    {
        if (!m_superclass)
            m_superclass = globalCtx.getClass(m_superclassIdentifier);
        
        return m_superclass;
    }

}