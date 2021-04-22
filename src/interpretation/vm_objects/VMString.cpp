#include <iostream>
#include "../Interpret.h"
#include "VMString.h"

namespace som {
    
    void VMString::dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret)
    {
        auto fnIt = stringPrimitives.find(selector);
        if (fnIt == stringPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = stringPrimitives.at(selector);
        interpret->executionStack().pushFrame(retAddress, arity + 1);
        fn(this, interpret);
    }
    
    void VMString::print(CInterpret* interpret)
    {
        auto receiver = interpret->executionStack().getArgument(0);
        std::string strVal = receiver->getValue().asString();
        std::cout << strVal;
        interpret->executionStack().push(receiver);
    }

    void VMString::printLn(CInterpret* interpret)
    {
        print(interpret);
        std::cout << std::endl;
    }

    void VMString::length(CInterpret* interpret)
    {
        auto receiver = interpret->executionStack().getArgument(0)->getValue().asString();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue((int32_t)receiver.size())));
    }

    void VMString::concatenate(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asString();
        auto receiver = interpret->executionStack().getArgument(1)->getValue().asString();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("String"), VMValue(receiver.append(argument))));
    }
    
    void VMString::isWhitespace(CInterpret* interpret)
    {
        std::cout << "String isWhitespace" << std::endl;
    }
    
    void VMString::isDigits(CInterpret* interpret)
    {
        std::cout << "String isDigits" << std::endl;
    }
        
    void VMString::compare(CInterpret* interpret)
    {
        auto& argument = interpret->executionStack().getArgument(0)->getValue().asString();
        auto& receiver = interpret->executionStack().getArgument(1)->getValue().asString();
        if (receiver == argument)
            interpret->executionStack().push(interpret->globalContext().getTrue());
        else
            interpret->executionStack().push(interpret->globalContext().getFalse());
    }

}