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
        std::shared_ptr<VMClass> getClass(const std::string& identifier) const;

        std::shared_ptr<VMClass>& getRunClass() { return m_runClass; }
    private:
        std::vector<std::shared_ptr<VMClass>> m_classes;
        std::shared_ptr<VMClass> m_runClass;

        VMClass* createClass(const Program* program, const ClassValue* classVal);
    };

}