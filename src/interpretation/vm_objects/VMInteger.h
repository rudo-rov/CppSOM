#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {

    class CInterpret;
    
    class VMInteger : public VMClass {
    public:
        VMInteger() : VMClass("Integer", "Object") {}

        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        // Implementations of primitive methods
        void asString(CInterpret* interpret);
        void plus(CInterpret* interpret);
        void minus(CInterpret* interpret);
        void mult(CInterpret* interpret);
        void div(CInterpret* interpret);
        void floatDiv(CInterpret* interpret);
        void mod(CInterpret* interpret);
        void cmp(CInterpret* interpret);

    private:

    };

    const std::map<std::string, std::function<void(VMInteger*, CInterpret*)>> integerPrimitives = {
        { "asString", std::mem_fn(&VMInteger::asString) },
        { "+", std::mem_fn(&VMInteger::plus) },
        { "-", std::mem_fn(&VMInteger::minus) },
        { "*", std::mem_fn(&VMInteger::mult) },
        { "/", std::mem_fn(&VMInteger::div) },
        { "//", std::mem_fn(&VMInteger::floatDiv) },
        { "%", std::mem_fn(&VMInteger::mod) },
        { "=", std::mem_fn(&VMInteger::cmp) }
    };

}