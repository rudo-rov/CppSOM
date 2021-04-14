#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"

namespace som {
    
    class VMString : public VMClass {
    public:
        VMString() : VMClass("String") {}

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack) override;

        // Implementation of primitive methods
        void print(CExecutionStack& stack);
        void printLn(CExecutionStack& stack);
        void length(CExecutionStack& stack);
        void concatenate(CExecutionStack& stack);
        void isWhitespace(CExecutionStack& stack);
        void isDigits(CExecutionStack& stack);
        // Comparations - binary meesage with selector '='
        void compare(CExecutionStack& stack);

    private:

    };

    // Standard mapping of primitive methods to their implementations
    const std::map<std::string, std::function<void(VMString*, CExecutionStack&)>> stringPrimitives = {
        { "print", std::mem_fn(&VMString::print) },
        { "printLn", std::mem_fn(&VMString::printLn) },
        { "length", std::mem_fn(&VMString::length) },
        { "concatenate", std::mem_fn(&VMString::concatenate) },
        { "isWhitespace", std::mem_fn(&VMString::isWhitespace) },
        { "isDigits", std::mem_fn(&VMString::isDigits) },
        { "=", std::mem_fn(&VMString::compare) }
    };

}
