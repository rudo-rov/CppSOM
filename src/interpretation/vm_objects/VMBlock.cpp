#include <iostream>
#include "../Interpret.h"
#include "VMBlock.h"

namespace som {

    void VMBlock::dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret)
    {
        auto fnIt = blockPrimitives.find(selector);
        if (fnIt == blockPrimitives.end()) {
            // Try to resolve in superclass
        }
        auto fn = blockPrimitives.at(selector);
        interpret->executionStack().pushFrame(retAddress, arity + 1);
        fn(this, interpret);
    }
    
    void VMBlock::value(CInterpret* interpret)
    {
        auto block = interpret->executionStack().getArgument(0);
        interpret->programCounter().setAddress(block->getValue().asBlockAddress());
        interpret->interpret();
    }

}