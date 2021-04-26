#pragma once
#include <cinttypes>
#include <stack>
#include <vector>
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
        void addArgument(std::shared_ptr<VMObject> obj);
        std::shared_ptr<VMObject> pop();
        std::shared_ptr<VMObject>& top() { return m_stack.back(); }

        std::shared_ptr<VMObject>& getArgument(int32_t idx);
        std::shared_ptr<VMObject>& getSelf();
        std::shared_ptr<VMObject>& fromTop(int32_t idx);
        void setLocal(int32_t idx, std::shared_ptr<VMObject>& value);
        std::shared_ptr<VMObject>& getLocal(int32_t idx);

        std::vector<std::shared_ptr<VMObject>>& getLocals() { return m_locals; }
        void setLocals(std::vector<std::shared_ptr<VMObject>>& locals);

    private:
        CodeAddress m_returnAddress;
        std::vector<std::shared_ptr<VMObject>> m_stack;
        std::vector<std::shared_ptr<VMObject>> m_args;
        std::vector<std::shared_ptr<VMObject>> m_locals;
    };

}