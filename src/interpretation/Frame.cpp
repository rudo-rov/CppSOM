#include <deque>
#include <cassert>
#include "Frame.h"

namespace som {

    void Frame::push(std::shared_ptr<VMObject>& obj)
    {
        m_stack.push_back(obj);
    }

    std::shared_ptr<VMObject> Frame::pop()
    {
        auto ret = m_stack.back();
        m_stack.pop_back();
        return ret;
    }

    void Frame::addArgument(std::shared_ptr<VMObject> obj)
    {
        m_args.push_back(std::move(obj));
    }

    std::shared_ptr<VMObject>& Frame::getArgument(int32_t idx)
    {
        assert(idx < m_args.size());
        return m_args.at(idx);
    }

    std::shared_ptr<VMObject>& Frame::fromTop(int32_t idx)
    {
        assert(m_stack.size() - idx - 1 >= 0);
        return m_stack.at(m_stack.size() - idx - 1);
    }

    std::shared_ptr<VMObject>& Frame::getSelf()
    {
        assert(m_args.size() > 0);
        return m_args.back();
    }

}