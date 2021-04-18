#pragma once
#include <string>
#include <vector>

#include "VMObject.h"
#include "../../bytecode/Program.h"
#include "../ExecutionStack.h"
#include "../Heap.h"

namespace som {

    class CGlobalContext;

    class VMClass : public VMObject {
    public:
        VMClass() {}
        VMClass(const std::string& identifier) : m_identifier(identifier), m_primitives() {}
        ~VMClass() = default;

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) {}
        
        virtual void addSlot(const Program* program, int32_t slotIdx);
        const std::string& className() const { return m_identifier; }

        // Method handling
        bool isPrimitive(const std::string& selector) const;

        // Object instantiation
        std::shared_ptr<VMObject> newObject(CHeap& heap);

    protected:
        std::string m_identifier;
        std::vector<std::string> m_primitives;

        virtual void addPrimitive(const Program* program, const Value* valPtr);
        void addMethod(const Program* program, const Value* valPtr);
    };

}