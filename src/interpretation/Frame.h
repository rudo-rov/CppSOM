#pragma once
#include <cinttypes>
#include <stack>
#include <memory>

#include "vm_objects/VMObject.h"
#include "ProgramCounter.h"

namespace som {

    class Frame {
    public:

        Frame() = default;
        Frame(CodeAddress retAddress) : m_returnAddress(retAddress) {}
        CodeAddress returnAddress() const { return m_returnAddress; }

        void push(std::shared_ptr<VMObject>& obj);
        std::shared_ptr<VMObject> pop();
        std::shared_ptr<VMObject> top() { return m_stack.top(); }

    private:
        CodeAddress m_returnAddress;
        std::stack<std::shared_ptr<VMObject>> m_stack;
    };

}