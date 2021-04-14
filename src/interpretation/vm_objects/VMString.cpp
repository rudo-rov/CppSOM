#include <iostream>
#include "VMString.h"

namespace som {
    
    void VMString::dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto fnIt = stringPrimitives.find(selector);
        if (fnIt == stringPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = stringPrimitives.at(selector);
        fn(this, stack, globalCtx);
    }
    
    void VMString::print(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto receiver = stack.top(); // Don't pop - function returns the callee
        std::string strVal = receiver->getValue().asString();
        std::cout << strVal;
    }

    void VMString::printLn(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        print(stack, globalCtx);
        std::cout << std::endl;
    }

    void VMString::length(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        std::cout << "String length" << std::endl;
    }

    void VMString::concatenate(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        std::cout << "String concatenate" << std::endl;
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