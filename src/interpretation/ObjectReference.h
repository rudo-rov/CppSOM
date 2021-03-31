#pragma once
#include <string>

#include "../bytecode/Bytecode.h"

namespace som {

    class CObjectReference {
    public:
        enum class EObjectTag {
            Int,
            String,
            Array,
            Nil,
            Custom
        };

        CObjectReference() : m_tag(EObjectTag::Nil) {}
        CObjectReference(EObjectTag tag) : m_tag(tag) {}
        virtual EObjectTag getTag() { return m_tag; }

        static CObjectReference* fromValue(Value* val); 

    protected:
        EObjectTag m_tag;
    };

    class CStringObjectReference : public CObjectReference {
    public:
        CStringObjectReference(std::string val) : CObjectReference(EObjectTag::String), m_val(val) {}
    private:
        std::string m_val;
    };

}