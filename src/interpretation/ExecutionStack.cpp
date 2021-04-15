#include <cassert>
#include "ExecutionStack.h"

namespace som {

    CExecutionStack::CExecutionStack()
    {
        Frame newFrame;
        m_stack.push(std::move(newFrame));
    }

    void CExecutionStack::pushFrame(CodeAddress retAddress, int nargs)
    {
        auto& oldFrame = m_stack.top();
        Frame newFrame(retAddress);
        m_stack.push(std::move(newFrame));

        for (int i = 0; i < nargs; ++i) {
            push(oldFrame.pop());
        }
    }

    CodeAddress CExecutionStack::popFrame()
    {
        CodeAddress retAddr = m_stack.top().returnAddress();
        m_stack.pop();
        return retAddr;
    }
    
    void CExecutionStack::push(std::shared_ptr<VMObject>& obj)
    {
        assert(!m_stack.empty());
        m_stack.top().push(obj);
    }

    std::shared_ptr<VMObject> CExecutionStack::pop()
    {
        assert(!m_stack.empty());
        return m_stack.top().pop();
    }

    std::shared_ptr<VMObject> CExecutionStack::getArgument(int32_t idx)
    {
        return m_stack.top().getArgument(idx);
    }

}