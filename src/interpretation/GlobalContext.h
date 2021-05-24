#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../bytecode/Program.h"
#include "vm_objects/VMClass.h"

namespace som {

    class CGlobalContext {
    public:
        CGlobalContext() {}
        ~CGlobalContext();

        void initialize(const Program* program);
        std::shared_ptr<VMClass>& getClass(const std::string& identifier);

        std::shared_ptr<VMClass>& getRunClass() { return m_runClass; }
        std::shared_ptr<VMObject>& getNil() { return m_nil; }
        std::shared_ptr<VMObject>& getTrue() { return m_true; }
        std::shared_ptr<VMObject>& getFalse() { return m_false; }

        std::shared_ptr<VMObject>& getObject(const std::string& identifier);

    private:
        std::vector<std::shared_ptr<VMClass>> m_classes;
        std::shared_ptr<VMObject> m_nil;
        std::shared_ptr<VMObject> m_true;
        std::shared_ptr<VMObject> m_false;
        std::shared_ptr<VMClass> m_runClass;

        VMClass* createClass(const Program* program, const ClassValue* classVal, std::string& runClass);
    };

}