#pragma once

#include <vector>
#include <memory>

#include "Bytecode.h"
#include "BCClass.h"

namespace som {

    class Program {
    public:

        Program() : m_entryPoint(-1) {}
        ~Program() {}
        
        // Serialization
        // Deserialization

        int32_t registerClass(const BCClass& newClass);
        size_t registerMethod(int32_t patternIdx, int32_t arity, int32_t nlocals, insVector* code);
        
        int32_t registerConstant(int32_t value);
        int32_t registerConstant(double value);
        int32_t registerConstant(const std::string& value);

        [[nodiscard]]
        int32_t indexOf(const std::string& value) const;

        bool setEntryPoint(int32_t entryPoint);

        void print();
        bool serialize(const std::string& fileName) const;

    private:
        // Constants pool
        std::vector<std::unique_ptr<Value>> m_constants;
        // Entry point
        int32_t m_entryPoint; // Idx of the method run
    };

}
