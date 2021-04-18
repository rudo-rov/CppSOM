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

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) override;

        // Implementations of primitive methods
        void asString(CExecutionStack& stack, CGlobalContext& globalCtx);
        void plus(CExecutionStack& stack, CGlobalContext& globalCtx);
        void minus(CExecutionStack& stack, CGlobalContext& globalCtx);
        void mult(CExecutionStack& stack, CGlobalContext& globalCtx);
        void div(CExecutionStack& stack, CGlobalContext& globalCtx);
        void floatDiv(CExecutionStack& stack, CGlobalContext& globalCtx);
        void mod(CExecutionStack& stack, CGlobalContext& globalCtx);

    private:

    };

    const std::map<std::string, std::function<void(VMInteger*, CExecutionStack&, CGlobalContext&)>> integerPrimitives = {
        { "asString", std::mem_fn(&VMInteger::asString) },
        { "+", std::mem_fn(&VMInteger::plus) },
        { "-", std::mem_fn(&VMInteger::minus) },
        { "*", std::mem_fn(&VMInteger::mult) },
        { "/", std::mem_fn(&VMInteger::div) },
        { "//", std::mem_fn(&VMInteger::floatDiv) },
        { "%", std::mem_fn(&VMInteger::mod) }
    };

}