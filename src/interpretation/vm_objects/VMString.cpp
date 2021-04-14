#include <iostream>
#include "VMString.h"

namespace som {
    
    void VMString::dispatchPrimitive(const std::string& selector, CExecutionStack& stack)
    {
        auto fnIt = stringPrimitives.find(selector);
        if (fnIt == stringPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = stringPrimitives.at(selector);
        fn(this, stack);
    }
    
    void VMString::print(CExecutionStack& stack)
    {
        auto receiver = stack.top(); // Don't pop - function returns the callee
        std::string strVal = receiver->getValue().asString();
        std::cout << strVal;
    }

    void VMString::printLn(CExecutionStack& stack)
    {
        print(stack);
        std::cout << std::endl;
    }

    void VMString::length(CExecutionStack& stack)
    {
        std::cout << "String length" << std::endl;
    }

    void VMString::concatenate(CExecutionStack& stack)
    {
        std::cout << "String concatenate" << std::endl;
    }
    
    void VMString::isWhitespace(CExecutionStack& stack)
    {
        std::cout << "String isWhitespace" << std::endl;
    }
    
    void VMString::isDigits(CExecutionStack& stack)
    {
        std::cout << "String isDigits" << std::endl;
    }
        
    void VMString::compare(CExecutionStack& stack)
    {
        std::cout << "String =" << std::endl;
    }

}