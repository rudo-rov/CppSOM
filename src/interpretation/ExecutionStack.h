#pragma once
#include <stack>

#include "Frame.h"


namespace som {

    class CExecutionStack {
    public:
        CExecutionStack();
        ~CExecutionStack() = default;

        // push and pop
        void push(std::shared_ptr<VMObject>& obj);
        std::shared_ptr<VMObject> pop();
        std::shared_ptr<VMObject> top() { return m_stack.top().top(); }
        
        void pushFrame(CodeAddress retAddress);
        CodeAddress popFrame();


    private:
        std::stack<Frame> m_stack;
    };

}