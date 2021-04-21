#pragma once
#include <map>
#include <functional>
#include "VMClass.h"

namespace som {

    class CInterpret;
    
    class VMBlock : public VMClass {
    public:
        VMBlock() : VMClass("Block") {}
        
        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        void value(CInterpret* interpret);

    };

    const std::map<std::string, std::function<void(VMBlock*, CInterpret*)>> blockPrimitives = {
        { "value", std::mem_fn(&VMBlock::value) }
    };

}