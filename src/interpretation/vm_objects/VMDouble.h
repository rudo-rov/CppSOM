#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {

    class CInterpret;
    
    class VMDouble : public VMClass {
    public:
        VMDouble() : VMClass("Double", "Object") {}

        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        // Implementations of primitive methods
        void asString(CInterpret* interpret);
        void plus(CInterpret* interpret);
        void minus(CInterpret* interpret);
        void mult(CInterpret* interpret);
        void div(CInterpret* interpret);

    private:
    };

    const std::map<std::string, std::function<void(VMDouble*, CInterpret*)>> doublePrimitives = {
        { "asString", std::mem_fn(&VMDouble::asString) },
        { "+", std::mem_fn(&VMDouble::plus) },
        { "-", std::mem_fn(&VMDouble::minus) },
        { "*", std::mem_fn(&VMDouble::mult) },
        { "/", std::mem_fn(&VMDouble::div) }
    };

}