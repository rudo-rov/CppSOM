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

    // CStringObjectReference
    void CStringObjectReference::dispatch(const std::string& methodIdentifier, CExecutionStack& stack)
    {
        // Temporary, there will be a better solution
        if (methodIdentifier == "print") {
            print();
        } else if (methodIdentifier == "printLn") {
            printLn();
        }
    }
    
    void CStringObjectReference::asString(CExecutionStack& stack) const
    {
        
    }

    void CStringObjectReference::print() const
    {
        std::cout << m_val;
    }

    void CStringObjectReference::printLn() const
    {
        std::cout << m_val << std::endl;
    }

    // CIntObjectReference
    void CIntObjectReference::asString(CExecutionStack& stack) const
    {
        stack.push(new CStringObjectReference(std::to_string(m_val)));
    }

}