#pragma once
#include <string>
#include <vector>

#include "VMObject.h"
#include "../../bytecode/Program.h"
#include "../ExecutionStack.h"

namespace som {

    class CGlobalContext;

    class VMClass : public VMObject {
    public:
        VMClass() {}
        VMClass(const std::string& identifier) : m_identifier(identifier), m_primitives() {}
        virtual ~VMClass() {}

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx) {}
        
        virtual void addSlot(const Program* program, int32_t slotIdx);
        const std::string& className() const { return m_identifier; }

        // Method handling
        bool isPrimitive(const std::string& selector) const;

    protected:
        std::string m_identifier;
        std::vector<std::string> m_primitives;

        virtual void addPrimitive(const Program* program, const Value* valPtr);
        void addMethod(const Program* program, const Value* valPtr);
    };

}