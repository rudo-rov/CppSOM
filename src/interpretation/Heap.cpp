#include <memory>
#include "Heap.h"

namespace som {

    std::shared_ptr<VMObject> CHeap::newObject(std::shared_ptr<VMClass>& clazz)
    {
        auto newObject = std::make_shared<VMObject>(clazz);
        m_objects.push_back(newObject);
        return newObject;
    }

    std::shared_ptr<VMObject> CHeap::newObject(std::shared_ptr<VMClass>& clazz, VMValue val)
    {
        auto newObject = std::make_shared<VMObject>(clazz, val);
        m_objects.push_back(newObject);
        return newObject;
    }

}