#include "VMInteger.h"

namespace som {

    void VMInteger::dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto fnIt = integerPrimitives.find(selector);
        if (fnIt == integerPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = integerPrimitives.at(selector);
        fn(this, stack, globalCtx);
    }

    void VMInteger::asString(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto receiver = stack.pop();
        int32_t intValue = receiver->getValue().asInt();
        // A way to retrieve string class?
        // stack.push(std::make_shared<VMObject>())
    }

}