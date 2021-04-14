#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"

namespace som {

    class VMInteger : public VMClass {
    public:
        VMInteger() : VMClass("Integer") {}

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack) override;

        // Implementations of primitive methods
        void asString(CExecutionStack& stack);

    private:

    };

    const std::map<std::string, std::function<void(VMInteger*, CExecutionStack&)>> integerPrimitives = {
        { "asString", std::mem_fn(&VMInteger::asString) }
    };

}