#pragma once
#include <memory>
#include <optional>
#include <unordered_map>
#include "../../bytecode/Bytecode.h"
#include "VMValue.h"
// #include "VMClass.h"

namespace som {

    class VMClass;
    class CGlobalContext;

    class VMObject {
    public:
        VMObject() : m_primValue(), m_class() {}
        VMObject(std::shared_ptr<VMClass> clazz) : m_class(clazz), m_primValue() {}
        VMObject(std::shared_ptr<VMClass> clazz, VMValue value) : m_class(clazz), m_primValue(value) {}
        VMObject(std::shared_ptr<VMClass> clazz, CGlobalContext& globalCtx);

        void setValue(VMValue val) { m_primValue = val; }

        const std::shared_ptr<VMClass> getClass() const { return m_class; }
        VMValue& getValue();
        std::shared_ptr<VMObject>& getField(const std::string& identifier) { return m_instanceFields[identifier]; }
        void setField(const std::string& identifier, std::shared_ptr<VMObject>& newValue);

    private:
        std::shared_ptr<VMClass> m_class;
        std::optional<VMValue> m_primValue;
        std::unordered_map<std::string, std::shared_ptr<VMObject>> m_instanceFields;

        void initializeInstanceFields(CGlobalContext& globalCtx);
    };

}