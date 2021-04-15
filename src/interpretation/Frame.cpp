#include <deque>
#include <cassert>
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

    void Frame::addArgument(std::shared_ptr<VMObject> obj)
    {
        m_args.push_back(std::move(obj));
    }

    std::shared_ptr<VMObject> Frame::getArgument(int32_t idx)
    {
        assert(idx < m_args.size());
        return m_args.at(idx);
    }

}