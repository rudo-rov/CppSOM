#pragma once
#include <stack>
#include <vector>
#include <memory>

#include "Frame.h"


namespace som {

    class CExecutionStack {
    public:
        CExecutionStack() {}
        ~CExecutionStack() = default;

        // push and pop
        void push(std::shared_ptr<VMObject> obj);
        std::shared_ptr<VMObject> pop();
        std::shared_ptr<VMObject> top() { return m_stack.back()->top(); }
        
        void pushFrame(CodeAddress retAddress, int nargs = 0, bool copyLocalRefs = false);
        void pushFrame(std::shared_ptr<Frame>& newFrame) { m_stack.push_back(newFrame); }
        Frame popFrame();
        Frame& topFrame() { return *m_stack.back(); }
        std::shared_ptr<Frame>& topFramePtr() { return m_stack.back(); }

        std::shared_ptr<VMObject>& getArgument(int32_t idx);
        std::shared_ptr<VMObject>& getSelf();
        std::shared_ptr<VMObject>& fromTop(int32_t idx) { return m_stack.back()->fromTop(idx); }

        std::shared_ptr<VMObject>& getLocal(int32_t idx) { return m_stack.back()->getLocal(idx); }
        void setLocal(int32_t idx, std::shared_ptr<VMObject> value) { m_stack.back()->setLocal(idx, value); }

        size_t size() const { return m_stack.size(); }
        bool empty() const { return m_stack.empty(); }

        std::vector<std::shared_ptr<Frame>>& getFrames() { return m_stack; }

    private:
        std::vector<std::shared_ptr<Frame>> m_stack;
    };

}