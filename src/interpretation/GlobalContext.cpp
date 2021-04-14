#include <string>
#include "GlobalContext.h"
#include "vm_objects/VMString.h"

namespace som {

    void CGlobalContext::initialize(const Program* program)
    {
        // For every class definition in the bytecode (CLASS value), instantiate a global class object
        for (const auto& val : program->getConstants()) {
            if (val->tag == ValueTag::ClassVal) {
                ClassValue* classVal = dynamic_cast<ClassValue*>(val.get());
                m_classes.emplace_back(createClass(program, classVal));
            }
        }
    }

    std::shared_ptr<VMClass> CGlobalContext::getClass(const std::string& identifier) const
    {
        for (const auto& clazz : m_classes) {
            if (clazz->className() == identifier)
                return clazz;
        }
        return nullptr;
    }

    VMClass* CGlobalContext::createClass(const Program* program, const ClassValue* classVal) const
    {
        // Get the class identifier - handles String, Boolean etc. in special cases
        std::string identifier = program->getStringValue(classVal->identifier);
        VMClass* newClass = nullptr;
        if (identifier == "String") {
           newClass = new VMString();
        } else {
            newClass = new VMClass();
        }
        for (int32_t slotIdx : classVal->slots) {
            newClass->addSlot(program, slotIdx);
        }
        return newClass;
        
    }

}