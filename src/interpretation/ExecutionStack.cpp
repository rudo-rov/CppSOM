#include <cassert>
#include "ExecutionStack.h"

namespace som {

    void CExecutionStack::pushFrame(CodeAddress retAddress, int nargs, bool copyLocalRefs)
    {
        if (nargs > 0) {
            auto& oldFrame = m_stack.top();
            Frame newFrame(retAddress);
            m_stack.push(std::move(newFrame));
            for (int i = 0; i < nargs; ++i) {
                m_stack.top().addArgument(oldFrame.pop());
            }
            if (copyLocalRefs) {
                newFrame.setLocals(oldFrame.getLocals());
            }
        } else {
            m_stack.emplace(retAddress);
        }
    }

    Frame CExecutionStack::popFrame()
    {
        auto ret = m_stack.top();
        m_stack.pop();
        return ret;
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

    std::shared_ptr<VMObject>& CExecutionStack::getArgument(int32_t idx)
    {
        return m_stack.top().getArgument(idx);
    }

    std::shared_ptr<VMObject>& CExecutionStack::getSelf()
    {
        return m_stack.top().getSelf();
    }

}