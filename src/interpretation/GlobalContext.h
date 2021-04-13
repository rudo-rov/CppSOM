#pragma once
#include <vector>
#include <memory>
#include "../bytecode/Program.h"
#include "vm_objects/VMClass.h"

namespace som {

    class CGlobalContext {
    public:
        CGlobalContext() {}

        void initialize(const Program* program);
    private:
        std::vector<std::shared_ptr<VMObject>> m_classes;

        VMClass* createClass(const Program* program, const ClassValue* classVal) const;
    };

}