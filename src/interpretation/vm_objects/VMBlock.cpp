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
        auto block = interpret->executionStack().getSelf();
        auto simpleRetAddress = interpret->executionStack().topFrame().returnAddress();
        interpret->programCounter().setAddress(block->getValue().asBlockContext().address);
        interpret->executionStack().popFrame();
        interpret->executionStack().pushFrame(block->getValue().asBlockContext().homeCtx);
        // interpret->executionStack().topFrame().redefineSelf(block);
        interpret->executionStack().topFrame().setLocalRetAddress(simpleRetAddress);
        interpret->interpret();
        block->getValue().asBlockContext().homeCtx->removeSelf();
    }

    void VMBlock::valueWithOneArg(CInterpret* interpret)
    {
        auto& block = interpret->executionStack().getSelf();
        interpret->programCounter().setAddress(block->getValue().asBlockContext().address);
        interpret->interpret();
    }

    void VMBlock::valueWithTwoArgs(CInterpret* interpret)
    {
        auto& block = interpret->executionStack().getSelf();
        interpret->programCounter().setAddress(block->getValue().asBlockContext().address);
        interpret->interpret();
    }

    void VMBlock::restart(CInterpret* interpret)
    {
        // Pop the new Frame
        interpret->executionStack().popFrame();
        // Set the PC to the initial address of that frame
        interpret->programCounter().setAddress(interpret->executionStack().topFrame().getInitialAddress());
        interpret->interpret();
    }

}