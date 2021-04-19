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
        

        size_t registerMethod(int32_t patternIdx, int32_t arity, int32_t nlocals, insVector* code);
        int32_t registerMethod(int32_t patternIdx, int32_t nargs); // Primitive methods
        int32_t registerClass(int32_t identifierIdx, std::vector<int32_t>& slots);
        int32_t registerBlock(int32_t nargs, insVector* code);
        
        int32_t registerConstant(int32_t value);
        int32_t registerConstant(double value);
        int32_t registerConstant(const std::string& value);

        [[nodiscard]]
        int32_t indexOf(const std::string& value);
        int32_t indexOf(int32_t value);
        int32_t indexOf(double value);

        bool setEntryPoint(int32_t entryPoint);
        std::vector<std::unique_ptr<ByteIns>>::iterator entryPoint();
        std::vector<std::unique_ptr<ByteIns>>::iterator exitPoint();

        void print();
        bool serialize(const std::string& fileName) const;

        Value* getValue(int32_t idx) const;
        std::string getStringValue(int32_t idx) const;

        const std::vector<std::unique_ptr<Value>>& getConstants() const { return m_constants; }

    private:
        // Constants pool
        std::vector<std::unique_ptr<Value>> m_constants;
        // Entry point
        int32_t m_entryPoint; // Idx of the method run
    };

}
