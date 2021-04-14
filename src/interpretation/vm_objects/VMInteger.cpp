#include "VMInteger.h"

namespace som {

    void VMInteger::dispatchPrimitive(const std::string& selector, CExecutionStack& stack)
    {
        auto fnIt = integerPrimitives.find(selector);
        if (fnIt == integerPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = integerPrimitives.at(selector);
        fn(this, stack);
    }

    void VMInteger::asString(CExecutionStack& stack)
    {
        auto receiver = stack.pop();
        
    }

}