#pragma once
#include <string>
#include <vector>
#include <map>

#include "VMObject.h"
#include "../../bytecode/Program.h"
#include "../ExecutionStack.h"
#include "../Heap.h"
#include "../GlobalContext.h"

namespace som {

    class CGlobalContext;

    class VMClass : public VMObject {
    public:
        VMClass() {}
        VMClass(const std::string& identifier) : m_identifier(identifier), m_primitives() {}
        ~VMClass() = default;

        virtual void dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx, CodeAddress retAddress, int32_t arity) {}
        CodeAddress getMethodAddr(const std::string& selector) { return m_methods[selector]; }
        
        virtual void addSlot(const Program* program, int32_t slotIdx);
        const std::string& className() const { return m_identifier; }

        // Method handling
        bool isPrimitive(const std::string& selector) const;

        // Object instantiation
        std::shared_ptr<VMObject> newObject(CHeap& heap, CGlobalContext& globalCtx);
        std::shared_ptr<VMObject> newObject(CHeap& heap, CGlobalContext& globalCtx, VMValue val);

    protected:
        std::string m_identifier;
        std::vector<std::string> m_primitives;
        std::map<std::string, CodeAddress> m_methods;

        virtual void addPrimitive(const Program* program, const Value* valPtr);
        void addMethod(const Program* program, const Value* valPtr);
    };

}