#pragma once
#include <map>
#include <functional>
#include "VMClass.h"

namespace som {

    class VMBlock : public VMClass {
    public:
        VMBlock() : VMClass("Block") {}
        
        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) override;

        void value(CExecutionStack& stack, CGlobalContext& globalCtx);

    };

    const std::map<std::string, std::function<void(VMBlock*, CExecutionStack&, CGlobalContext&)>> blockPrimitives = {
        { "value", std::mem_fn(&VMBlock::value) }
    };

}