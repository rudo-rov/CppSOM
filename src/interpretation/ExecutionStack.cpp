#include <cassert>
#include "ExecutionStack.h"

namespace som {

    CExecutionStack::CExecutionStack()
    {
        Frame newFrame;
        m_stack.push(std::move(newFrame));
    }
    
    void CExecutionStack::push(CObjectReference* obj)
    {
        assert(!m_stack.empty());
        m_stack.front().push(obj);
    }

}