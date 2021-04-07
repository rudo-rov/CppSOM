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
        std::shared_ptr<CObjectReference> pop();
        
        void pushFrame(CodeAddress retAddress);
        CodeAddress popFrame();


    private:
        std::queue<Frame> m_stack;
    };

}