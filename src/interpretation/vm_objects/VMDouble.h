#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {

    class VMDouble : public VMClass {
    public:
        VMDouble() : VMClass("Double") {}

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) override;

        // Implementations of primitive methods
        void asString(CExecutionStack& stack, CGlobalContext& globalCtx);
        void plus(CExecutionStack& stack, CGlobalContext& globalCtx);
        void minus(CExecutionStack& stack, CGlobalContext& globalCtx);
        void mult(CExecutionStack& stack, CGlobalContext& globalCtx);
        void div(CExecutionStack& stack, CGlobalContext& globalCtx);

    private:
    };

    const std::map<std::string, std::function<void(VMDouble*, CExecutionStack&, CGlobalContext&)>> doublePrimitives = {
        { "asString", std::mem_fn(&VMDouble::asString) },
        { "+", std::mem_fn(&VMDouble::plus) },
        { "-", std::mem_fn(&VMDouble::minus) },
        { "*", std::mem_fn(&VMDouble::mult) },
        { "/", std::mem_fn(&VMDouble::div) }
    };

}