#pragma once

namespace som {

    class CObjectReference {
    public:
        enum class EObjectTag {
            Int,
            String,
            Array,
            Nil
        };

        CObjectReference() : m_tag(EObjectTag::Nil) {}
        EObjectTag getTag() { return m_tag; } 

    protected:
        EObjectTag m_tag;
    };

    class CStringObjectReference {
    public:

    private:
        
    }

}