#include <deque>
#include "Frame.h"

namespace som {

    void Frame::push(std::shared_ptr<VMObject>& obj)
    {
        m_stack.push(obj);
    }

    std::shared_ptr<VMObject> Frame::pop()
    {
        auto ret = m_stack.top();
        m_stack.pop();
        return ret;
    }

}