#include <cassert>
#include "ExecutionStack.h"

namespace som {

    CExecutionStack::CExecutionStack()
    {
        Frame newFrame;
        m_stack.push(std::move(newFrame));
    }

    void CExecutionStack::pushFrame(CodeAddress retAddress)
    {
        Frame newFrame(retAddress);
        m_stack.push(std::move(newFrame));
    }

    CodeAddress CExecutionStack::popFrame()
    {
        CodeAddress retAddr = m_stack.front().returnAddress();
        m_stack.pop();
        return retAddr;
    }
    
    void CExecutionStack::push(CObjectReference* obj)
    {
        assert(!m_stack.empty());
        m_stack.front().push(obj);
    }

    std::shared_ptr<CObjectReference> CExecutionStack::pop()
    {
        assert(!m_stack.empty());
        return m_stack.front().pop();
    }

}