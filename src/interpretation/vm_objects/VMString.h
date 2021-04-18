#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {
    
    class VMString : public VMClass {
    public:
        VMString() : VMClass("String") {}
        ~VMString() = default;

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) override;

        // Implementation of primitive methods
        void print(CExecutionStack& stack, CGlobalContext& globalCtx);
        void printLn(CExecutionStack& stack, CGlobalContext& globalCtx);
        void length(CExecutionStack& stack, CGlobalContext& globalCtx);
        void concatenate(CExecutionStack& stack, CGlobalContext& globalCtx);
        void isWhitespace(CExecutionStack& stack, CGlobalContext& globalCtx);
        void isDigits(CExecutionStack& stack, CGlobalContext& globalCtx);
        // Comparations - binary meesage with selector '='
        void compare(CExecutionStack& stack, CGlobalContext& globalCtx);

    private:

    };

    // Standard mapping of primitive methods to their implementations
    const std::map<std::string, std::function<void(VMString*, CExecutionStack&, CGlobalContext&)>> stringPrimitives = {
        { "print", std::mem_fn(&VMString::print) },
        { "printLn", std::mem_fn(&VMString::printLn) },
        { "length", std::mem_fn(&VMString::length) },
        { "concatenate:", std::mem_fn(&VMString::concatenate) },
        { "isWhitespace", std::mem_fn(&VMString::isWhitespace) },
        { "isDigits", std::mem_fn(&VMString::isDigits) },
        { "=", std::mem_fn(&VMString::compare) }
    };

}
