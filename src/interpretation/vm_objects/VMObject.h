#pragma once
#include <memory>
#include "../../bytecode/Bytecode.h"
// #include "VMClass.h"

namespace som {

    class VMClass;

    class VMObject {
    public:
        VMObject() : m_value(nullptr), m_class(nullptr) {}
        VMObject(std::shared_ptr<VMClass> clazz, Value* val = nullptr) : m_class(clazz), m_value(val) {}

        void setValue(Value* val) { m_value = val; }

        const std::shared_ptr<VMClass> getClass() const { return m_class; }
        const Value* getValue() const { return m_value; }

    private:
        std::shared_ptr<VMClass> m_class;
        Value* m_value;

    };

}