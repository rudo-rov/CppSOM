#include <string>
#include <iostream>
#include "GlobalContext.h"
#include "vm_objects/VMString.h"
#include "vm_objects/VMInteger.h"
#include "vm_objects/VMBlock.h"
#include "vm_objects/VMArray.h"

namespace som { 

    CGlobalContext::~CGlobalContext()
    {
        for (auto& ptr : m_classes){
            ptr.reset();
        }
    }

    void CGlobalContext::initialize(const Program* program)
    {
        // For every class definition in the bytecode (CLASS value), instantiate a global class object
        std::string runClassIdentifier;
        for (const auto& val : program->getConstants()) {
            if (val->tag == ValueTag::ClassVal) {
                ClassValue* classVal = dynamic_cast<ClassValue*>(val.get());
                m_classes.emplace_back(createClass(program, classVal, runClassIdentifier));
            }
        }
        m_runClass = getClass(runClassIdentifier);

        // Create special global instances - true, false, nil
        auto& nilClass = getClass("Nil");
        if (nilClass) {
            m_nil = std::make_shared<VMObject>(nilClass);
        }

        auto& trueClass = getClass("True");
        if (trueClass) {
            m_true = std::make_shared<VMObject>(trueClass);
        }

        auto& falseClass = getClass("False");
        if (falseClass) {
            m_false = std::make_shared<VMObject>(falseClass);
        }
    }

    std::shared_ptr<VMClass>& CGlobalContext::getClass(const std::string& identifier)
    {
        for (auto& clazz : m_classes) {
            if (clazz->className() == identifier)
                return clazz;
        }
        std::cout << "Unresolved class name: " << identifier << std::endl;
        exit(0);
    }

    VMClass* CGlobalContext::createClass(const Program* program, const ClassValue* classVal, std::string& runClass)
    {
        // Get the class identifier - handles String, Boolean etc. in special cases
        std::string identifier = program->getStringValue(classVal->identifier);
        std::string superclass;
        if (classVal->superclass >= 0) {
            superclass = program->getStringValue(classVal->superclass);
        } else {
            superclass = "Object";
        }
        VMClass* newClass = nullptr;
        if (identifier == "String") {
           newClass = new VMString();
        } else if (identifier == "Integer") {
            newClass = new VMInteger();
        } else if (identifier == "Block") {
            newClass = new VMBlock();
        } else if (identifier == "Array") {
            newClass = new VMArray();
        } else {
            newClass = new VMClass(identifier, superclass);
        }
        for (int32_t slotIdx : classVal->slots) {
            newClass->addSlot(program, slotIdx);
            MethodValue* method = dynamic_cast<MethodValue*>(program->getValue(slotIdx));
            if (method) {
                if (program->getStringValue(method->name) == "run" && method->nargs == 0) {
                    runClass = identifier;
                }
            }
        }
        return newClass;
        
    }

    std::shared_ptr<VMObject>& CGlobalContext::getObject(const std::string& identifier)
    {
        if (identifier == "nil")
            return getNil();
        else if (identifier == "true")
            return getTrue();
        else if (identifier == "false")
            return getFalse();
        else {
            std::cout << "Unresolved identifier: " << identifier << std::endl;
            exit(1);
        }

    }

}