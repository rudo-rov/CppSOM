#pragma once
#include <stack>

#include "Frame.h"


namespace som {

    class CExecutionStack {
    public:
        CExecutionStack() {}
        ~CExecutionStack() = default;

        // push and pop
        void push(std::shared_ptr<VMObject>& obj);
        std::shared_ptr<VMObject> pop();
        std::shared_ptr<VMObject> top() { return m_stack.top().top(); }
        
        void pushFrame(CodeAddress retAddress, int nargs = 0);
        Frame popFrame();
        Frame& topFrame() { return m_stack.top(); }

        std::shared_ptr<VMObject>& getArgument(int32_t idx);
        std::shared_ptr<VMObject>& getSelf();
        std::shared_ptr<VMObject>& fromTop(int32_t idx) { return m_stack.top().fromTop(idx); }

        size_t size() const { return m_stack.size(); }

    private:
        std::stack<Frame> m_stack;
    };

}