#include <string>
#include "GlobalContext.h"
#include "vm_objects/VMString.h"
#include "vm_objects/VMInteger.h"

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

    VMClass* CGlobalContext::createClass(const Program* program, const ClassValue* classVal)
    {
        // Get the class identifier - handles String, Boolean etc. in special cases
        std::string identifier = program->getStringValue(classVal->identifier);
        VMClass* newClass = nullptr;
        if (identifier == "String") {
           newClass = new VMString();
        } else if (identifier == "Integer") {
            newClass = new VMInteger();
        } else {
            newClass = new VMClass();
        }
        for (int32_t slotIdx : classVal->slots) {
            newClass->addSlot(program, slotIdx);
            MethodValue* method = dynamic_cast<MethodValue*>(program->getValue(slotIdx));
            if (method) {
                if (program->getStringValue(method->name) == "run" && method->nargs == 0) {
                    m_runClass = std::shared_ptr<VMClass>(newClass);
                }
            }
        }
        return newClass;
        
    }

}