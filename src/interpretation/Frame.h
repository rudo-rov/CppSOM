#pragma once
#include <cinttypes>
#include <deque>
#include <memory>

#include "ObjectReference.h"
#include "ProgramCounter.h"

namespace som {

    class Frame {
    public:

        Frame() = default;
        Frame(CodeAddress retAddress) : m_returnAddress(retAddress) {}
        CodeAddress returnAddress() const { return m_returnAddress; }

        void push(CObjectReference* obj);
        std::shared_ptr<CObjectReference> pop();

    private:
        CodeAddress m_returnAddress;
        std::deque<std::shared_ptr<CObjectReference>> m_stack;
    };

}