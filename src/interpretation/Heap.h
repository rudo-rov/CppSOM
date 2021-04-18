#pragma once
#include <vector>
#include <memory>
#include "vm_objects/VMObject.h"

namespace som {

    class CHeap {
    public:
        CHeap() {}
        ~CHeap() = default;

        std::shared_ptr<VMObject> newObject(VMClass* clazz);

    private:
        std::vector<std::shared_ptr<VMObject>> m_objects;
    };

}