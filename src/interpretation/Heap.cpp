#include <memory>
#include "Heap.h"

namespace som {

    std::shared_ptr<VMObject>& CHeap::newObject(std::shared_ptr<VMClass>& clazz)
    {
        m_objects.emplace_back(std::make_shared<VMObject>(clazz));
        return m_objects.back();
    }

    std::shared_ptr<VMObject>& CHeap::newObject(std::shared_ptr<VMClass>& clazz, VMValue val)
    {
        m_objects.emplace_back(std::make_shared<VMObject>(clazz, val));
        return m_objects.back();
    }

    std::shared_ptr<VMObject>& CHeap::newObject(std::shared_ptr<VMClass>& clazz, CGlobalContext& globalCtx)
    {
        m_objects.emplace_back(std::make_shared<VMObject>(clazz, globalCtx));
        return m_objects.back();
    }

}