#include <cassert>
#include "VMObject.h"
#include "VMClass.h"

namespace som {

    VMObject::VMObject(std::shared_ptr<VMClass>& clazz, CGlobalContext& globalCtx) : m_class(clazz), m_primValue()
    {
        initializeInstanceFields(globalCtx);
    }
    
    // Should only be called on objects with primitive values - Strings, Integers, Bools etc.
    VMValue& VMObject::getValue()
    {
        assert(m_primValue);
        return m_primValue.value();
    }

    void VMObject::initializeInstanceFields(CGlobalContext& globalCtx)
    {
        assert(m_class);
        for (const auto& field : m_class->instanceFields()) {
            m_instanceFields[field] = globalCtx.getNil();
        }
    }

    void VMObject::setField(const std::string& identifier, std::shared_ptr<VMObject>& newValue)
    {
        assert(m_instanceFields.find(identifier) != m_instanceFields.end());
        m_instanceFields[identifier] = newValue;
    }

}