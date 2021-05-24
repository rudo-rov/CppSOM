#pragma once
#include <map>
#include <functional>
#include "VMClass.h"

namespace som {

    class CInterpret;

    class VMArray : public VMClass {
    public:
        VMArray() : VMClass("Array", "Object") {}

        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        void at(CInterpret* interpret);
        void atPut(CInterpret* interpret);
        void length(CInterpret* interpret);

    };

    const std::map<std::string, std::function<void(VMArray*, CInterpret*)>> arrayPrimitives = {
        { "at:", std::mem_fn(&VMArray::at) },
        { "at:put:", std::mem_fn(&VMArray::atPut) },
        { "length", std::mem_fn(&VMArray::length) }
    };

}