#include <cassert>
#include "ExecutionStack.h"

namespace som {

    void CExecutionStack::pushFrame(CodeAddress retAddress, int nargs, bool copyLocalRefs)
    {
        if (nargs > 0) {
            auto oldFrame = m_stack.back();
            Frame newFrame(retAddress);
            m_stack.push_back(std::make_shared<Frame>(retAddress));
            for (int i = 0; i < nargs; ++i) {
                m_stack.back()->addArgument(oldFrame->pop());
            }
            if (copyLocalRefs) {
                m_stack.back()->setLocals(oldFrame->getLocals());
            }
        } else {
            m_stack.emplace_back(std::make_shared<Frame>(retAddress));
        }
    }

    Frame CExecutionStack::popFrame()
    {
        auto ret = m_stack.back();
        m_stack.erase(m_stack.end() - 1);
        return *ret;
    }
    
    void CExecutionStack::push(std::shared_ptr<VMObject> obj)
    {
        assert(!m_stack.empty());
        m_stack.back()->push(obj);
    }

    std::shared_ptr<VMObject> CExecutionStack::pop()
    {
        assert(!m_stack.empty());
        return m_stack.back()->pop();
    }

    std::shared_ptr<VMObject>& CExecutionStack::getArgument(int32_t idx)
    {
        return m_stack.back()->getArgument(idx);
    }

    std::shared_ptr<VMObject>& CExecutionStack::getSelf()
    {
        return m_stack.back()->getSelf();
    }

}