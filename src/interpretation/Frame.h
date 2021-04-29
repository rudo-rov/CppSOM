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

        Frame() = delete;
        Frame(CodeAddress retAddress) : m_returnAddress(retAddress) {}
        CodeAddress returnAddress() const { return m_returnAddress; }
        CodeAddress localReturnAddress() const { return m_localRetAddress; }
        void setLocalRetAddress(CodeAddress& addr) { m_localRetAddress = addr; }

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

        void setInitialAddress(const CodeAddress& addr) { m_initialAddress = addr; }
        CodeAddress& getInitialAddress() { return m_initialAddress; }
        void setReturnAddress(const CodeAddress& addr) { m_returnAddress = addr; }

        bool localsCopied() { return m_localsCopied; }
        void redefineSelf(std::shared_ptr<VMObject>& newSelf) { m_args.push_back(newSelf); }
        void removeSelf() { m_args.erase(m_args.end() - 1); }


    private:
        CodeAddress m_returnAddress;
        CodeAddress m_initialAddress;
        CodeAddress m_localRetAddress;
        std::vector<std::shared_ptr<VMObject>> m_stack;
        std::vector<std::shared_ptr<VMObject>> m_args;
        std::vector<std::shared_ptr<VMObject>> m_locals;
        bool m_localsCopied = false;
    };

}