#include <deque>
#include "Frame.h"
#include "ObjectReference.h"

namespace som {

    void Frame::push(CObjectReference* obj)
    {
        m_stack.emplace_front(obj);
    }

}