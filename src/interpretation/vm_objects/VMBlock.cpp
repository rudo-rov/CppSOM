#include <iostream>
#include "VMBlock.h"

namespace som {

    void VMBlock::dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity)
    {
        auto fnIt = blockPrimitives.find(selector);
        if (fnIt == blockPrimitives.end()) {
            // Try to resolve in superclass
        }
        auto fn = blockPrimitives.at(selector);
        stack.pushFrame(retAddress, arity + 1);
        fn(this, stack, globalCtx);
    }
    
    void VMBlock::value(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        
        std::cout << "Value on a block has been called.\n";
    }

}