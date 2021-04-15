#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {

    class VMInteger : public VMClass {
    public:
        VMInteger() : VMClass("Integer") {}

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx) override;

        // Implementations of primitive methods
        void asString(CExecutionStack& stack, CGlobalContext& globalCtx);
        void plus(CExecutionStack& stack, CGlobalContext& globalCtx);

    private:

    };

    const std::map<std::string, std::function<void(VMInteger*, CExecutionStack&, CGlobalContext&)>> integerPrimitives = {
        { "asString", std::mem_fn(&VMInteger::asString) },
        { "+", std::mem_fn(&VMInteger::plus) }
    };

}