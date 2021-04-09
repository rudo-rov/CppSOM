#include <string>
#include <iostream>
#include "ObjectReference.h"
#include "ExecutionStack.h"

namespace som {

    /* static */ CObjectReference* CObjectReference::fromValue(Value* val)
    {
        switch (val->tag)
        {
        case ValueTag::StringVal: {
            StringValue* strVal = static_cast<StringValue*>(val);
            return new CStringObjectReference(strVal->value);
        }
            
        
        default:
            return new CObjectReference();
        }
    }

    // Should return an address to the first instruction of method to execute
    // Somehow should handle "primitives" - method to be executed by the VM - print, asString etc...
    void CObjectReference::dispatch(const std::string& methodIdentifier, CExecutionStack& stack)
    {
        
    }

    bool CObjectReference::isPrimitive(const std::string& methodIdentifier) const
    {
        // TODO
        return true;
    }

    // CClassObjectReference
    void CClassObjectReference::initialize(ClassValue* classVal, Program* program)
    {
        StringValue* identifierVal = dynamic_cast<StringValue*>(program->getValue(classVal->identifier));
        m_identifier = identifierVal->value;
        for (int32_t slot : classVal->slots) {
            Value* slotPtr = program->getValue(slot);
            StringValue* strVal = dynamic_cast<StringValue*>(slotPtr);
            if (strVal) { // fields
                m_fields[strVal->value] = CObjectReference();
            }
        }
    }

    // CStringObjectReference
    void CStringObjectReference::dispatch(const std::string& methodIdentifier, CExecutionStack& stack)
    {
        // Temporary, there will be a better solution
        if (methodIdentifier == "print") {
            print(stack);
        } else if (methodIdentifier == "printLn") {
            printLn(stack);
        }
    }
    
    void CStringObjectReference::print(CExecutionStack& stack) const
    {
        std::cout << m_val;
    }

    void CStringObjectReference::printLn(CExecutionStack& stack) const
    {
        std::cout << m_val << std::endl;
    }

    // CIntObjectReference
    void CIntObjectReference::asString(CExecutionStack& stack) const
    {
        stack.push(new CStringObjectReference(std::to_string(m_val)));
    }

}