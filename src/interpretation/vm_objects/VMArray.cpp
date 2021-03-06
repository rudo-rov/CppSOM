#include "VMArray.h"
#include "../Interpret.h"

namespace som {

    void VMArray::dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret)
    {
        auto fnIt = arrayPrimitives.find(selector);
        if (fnIt == arrayPrimitives.end()) {
            return;
        }
        auto fn = fnIt->second;
        interpret->executionStack().pushFrame(retAddress, arity + 1);
        fn(this, interpret);
    }

    void VMArray::at(CInterpret* interpret)
    {
        auto& arr = interpret->executionStack().getSelf()->getValue().asVector();
        int32_t idx = interpret->executionStack().getArgument(0)->getValue().asInt();
        if (idx > arr.size()) {
            interpret->executionStack().push(interpret->globalContext().getNil());
        } else {
            interpret->executionStack().push(arr.at(idx - 1));
        }
    }

    void VMArray::atPut(CInterpret* interpret)
    {
        auto& arr = interpret->executionStack().getSelf()->getValue().asVector();
        int32_t idx = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto& newVal = interpret->executionStack().getArgument(1);
        if (idx <= arr.size()) {
            arr[idx - 1] = newVal;
        }
        // Returns the array being modified
        interpret->executionStack().push(interpret->executionStack().getSelf());
    }

    void VMArray::length(CInterpret* interpret)
    {
        auto& obj = interpret->executionStack().getSelf();
        auto& arr = interpret->executionStack().getSelf()->getValue().asVector();
        int32_t size = arr.size();
        auto& intClass = interpret->globalContext().getClass("Integer");
        interpret->executionStack().push(std::make_shared<VMObject>(intClass, size));
    }

}