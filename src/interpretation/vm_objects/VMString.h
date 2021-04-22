#pragma once
#include <map>
#include <functional>
#include "VMClass.h"
#include "../ExecutionStack.h"
#include "../GlobalContext.h"

namespace som {
    
    class VMString : public VMClass {
    public:
        VMString() : VMClass("String", "Object") {}
        ~VMString() = default;

        virtual void dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret) override;

        // Implementation of primitive methods
        void print(CInterpret* interpret);
        void printLn(CInterpret* interpret);
        void length(CInterpret* interpret);
        void concatenate(CInterpret* interpret);
        void isWhitespace(CInterpret* interpret);
        void isDigits(CInterpret* interpret);
        // Comparations - binary meesage with selector '='
        void compare(CInterpret* interpret);

    private:

    };

    // Standard mapping of primitive methods to their implementations
    const std::map<std::string, std::function<void(VMString*, CInterpret*)>> stringPrimitives = {
        { "print", std::mem_fn(&VMString::print) },
        { "printLn", std::mem_fn(&VMString::printLn) },
        { "length", std::mem_fn(&VMString::length) },
        { "concatenate:", std::mem_fn(&VMString::concatenate) },
        { "isWhitespace", std::mem_fn(&VMString::isWhitespace) },
        { "isDigits", std::mem_fn(&VMString::isDigits) },
        { "=", std::mem_fn(&VMString::compare) }
    };

}
