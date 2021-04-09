#pragma once
#include <string>
#include <unordered_map>

#include "../bytecode/Bytecode.h"
#include "ProgramCounter.h"
#include "../bytecode/Program.h"
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
            Class,
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

    class CClassObjectReference : CObjectReference {
    public:
        CClassObjectReference(ClassValue* classVal, Program* program) : CObjectReference(EObjectTag::Class) { initialize(classVal, program); }
        void initialize(ClassValue* classVal, Program* program);
        
    private:
        std::string m_identifier;
        std::unordered_map<std::string, CObjectReference> m_fields;
        std::unordered_map<std::string, CodeAddress> m_methods;
        
    };

    class CStringObjectReference : public CObjectReference {
    public:
        CStringObjectReference(std::string val) : CObjectReference(EObjectTag::String), m_val(val) {}
        void dispatch(const std::string& methodIdentifier, CExecutionStack& stack) override;
        void asString(CExecutionStack& stack) const;
        
        // CodeAddress method(int32_t idx) override;
        void print(CExecutionStack& stack) const;
        void printLn(CExecutionStack& stack) const;
        void length(CExecutionStack& stack) const;

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