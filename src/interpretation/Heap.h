#pragma once
#include <vector>
#include <memory>
#include "vm_objects/VMObject.h"

namespace som {

    class CHeap {
    public:
        CHeap() {}
        ~CHeap() = default;

        std::shared_ptr<VMObject> newObject(std::shared_ptr<VMClass>& clazz);
        std::shared_ptr<VMObject> newObject(std::shared_ptr<VMClass>& clazz, VMValue val);

    private:
        std::vector<std::shared_ptr<VMObject>> m_objects;
    };

}