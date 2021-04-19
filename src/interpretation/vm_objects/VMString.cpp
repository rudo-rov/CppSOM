#include <iostream>
#include "VMString.h"

namespace som {
    
    void VMString::dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity)
    {
        auto fnIt = stringPrimitives.find(selector);
        if (fnIt == stringPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = stringPrimitives.at(selector);
        stack.pushFrame(retAddress, arity + 1); // +1 because of the receiver of the message
        fn(this, stack, globalCtx);
    }
    
    void VMString::print(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto receiver = stack.getArgument(0);
        std::string strVal = receiver->getValue().asString();
        std::cout << strVal;
        stack.push(receiver);
    }

    void VMString::printLn(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        print(stack, globalCtx);
        std::cout << std::endl;
    }

    void VMString::length(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto receiver = stack.getArgument(0)->getValue().asString();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("Integer"), VMValue((int32_t)receiver.size())));
    }

    void VMString::concatenate(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto argument = stack.getArgument(0)->getValue().asString();
        auto receiver = stack.getArgument(1)->getValue().asString();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("String"), VMValue(receiver.append(argument))));
    }
    
    void VMString::isWhitespace(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        std::cout << "String isWhitespace" << std::endl;
    }
    
    void VMString::isDigits(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        std::cout << "String isDigits" << std::endl;
    }
        
    void VMString::compare(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        std::cout << "String =" << std::endl;
    }

}