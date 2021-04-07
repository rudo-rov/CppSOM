#pragma once
#include <string>
#include <unordered_map>

#include "../bytecode/Bytecode.h"
#include "ProgramCounter.h"
// #include "ExecutionStack.h"

namespace som {

    class CExecutionStack;

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
        virtual void dispatch(const std::string& methodIdentifier, CExecutionStack& stack);
        bool isPrimitive(const std::string& methodIdentifier) const;       

        static CObjectReference* fromValue(Value* val); 

    protected:
        EObjectTag m_tag;
        // std::map
    };

    class CStringObjectReference : public CObjectReference {
    public:
        CStringObjectReference(std::string val) : CObjectReference(EObjectTag::String), m_val(val) {}
        void dispatch(const std::string& methodIdentifier, CExecutionStack& stack) override;
        void asString(CExecutionStack& stack) const;
        
        // CodeAddress method(int32_t idx) override;
        void print() const;
        void printLn() const;

    private:
        std::string m_val;
    };

    class CIntObjectReference : public CObjectReference {
    public:
        CIntObjectReference(int32_t val) : CObjectReference(EObjectTag::Int), m_val(val) {}

        void asString(CExecutionStack& stack) const;

    private:
        int32_t m_val;
    };

}