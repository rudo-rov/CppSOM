#include <deque>
#include "Frame.h"
#include "ObjectReference.h"

namespace som {

    void Frame::push(CObjectReference* obj)
    {
        m_stack.emplace_front(obj);
    }

    std::unique_ptr<CObjectReference>&& Frame::pop()
    {
        auto ret = std::move(m_stack.front());
        m_stack.pop_front();
        return std::move(ret);
    }

}