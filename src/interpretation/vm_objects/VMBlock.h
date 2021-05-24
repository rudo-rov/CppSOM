#pragma once
#include <map>
#include <functional>
#include "VMClass.h"

namespace som {

    class CInterpret;
    
    class VMBlock : public VMClass {
    public:
        VMBlock() : VMClass("Block", "Object") {}
        
        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        void value(CInterpret* interpret);
        void valueWithOneArg(CInterpret* interpret);
        void valueWithTwoArgs(CInterpret* interpret);
        void restart(CInterpret* interpret);

    };

    const std::map<std::string, std::function<void(VMBlock*, CInterpret*)>> blockPrimitives = {
        { "value", std::mem_fn(&VMBlock::value) },
        { "value:", std::mem_fn(&VMBlock::valueWithOneArg) },
        { "value:value:", std::mem_fn(&VMBlock::valueWithTwoArgs) },
        { "restart", std::mem_fn(&VMBlock::restart) },
    };

}