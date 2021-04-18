#include "Heap.h"

namespace som {

    std::shared_ptr<VMObject> CHeap::newObject(VMClass* clazz)
    {
        auto newObject = std::make_shared<VMObject>(std::shared_ptr<VMClass>(clazz));
        m_objects.push_back(newObject);
        return newObject;
    }

}