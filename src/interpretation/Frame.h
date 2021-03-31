#pragma once
#include <cinttypes>
#include <deque>
#include <memory>

#include "ObjectReference.h"

namespace som {

    class Frame {
    public:

        int32_t returnAddress() const { return m_returnAddress; }

        void push(CObjectReference* obj);
        

    private:
        int32_t m_returnAddress;
        std::deque<std::unique_ptr<CObjectReference>> m_stack;
    };

}