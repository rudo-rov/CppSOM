#pragma once
#include <queue>

#include "Frame.h"

namespace som {

    class CExecutionStack {
    public:
        CExecutionStack();
        ~CExecutionStack() = default;

        // push and pop
        void push(CObjectReference* obj);

    private:
        std::queue<Frame> m_stack;
    };

}