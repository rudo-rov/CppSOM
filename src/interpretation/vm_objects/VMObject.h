#pragma once
#include <memory>
#include <optional>
#include "../../bytecode/Bytecode.h"
#include "VMValue.h"
// #include "VMClass.h"

namespace som {

    class VMClass;

    class VMObject {
    public:
        VMObject() : m_primValue(), m_class() {}
        VMObject(std::shared_ptr<VMClass> clazz) : m_class(clazz), m_primValue() {}
        VMObject(std::shared_ptr<VMClass> clazz, VMValue value) : m_class(clazz), m_primValue(value) {}

        void setValue(VMValue val) { m_primValue = val; }

        const std::shared_ptr<VMClass> getClass() const { return m_class; }
        VMValue getValue() const;

    private:
        std::shared_ptr<VMClass> m_class;
        std::optional<VMValue> m_primValue;

    };

}