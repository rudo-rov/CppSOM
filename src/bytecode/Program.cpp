#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include "Program.h"
#include "Bytecode.h"

namespace som {
    
    int32_t Program::registerClass(const BCClass& newClass)
    {
        ClassValue* newClassVal = new ClassValue(std::move(newClass.slots()));
        m_constants.emplace_back(newClassVal);
        return m_constants.size() - 1;
    }
    
    int32_t Program::registerConstant(int32_t value)
    {
        m_constants.emplace_back(new IntValue(value));
        return m_constants.size() - 1;
    }

    int32_t Program::registerConstant(double value)
    {
        m_constants.emplace_back(new DoubleValue(value));
        return m_constants.size() - 1;
    }

    int32_t Program::registerConstant(const std::string& value)
    {
        m_constants.emplace_back(new StringValue(value));
        return m_constants.size() - 1;
    }

    size_t Program::registerMethod(int32_t patternIdx, int32_t arity, int32_t nlocals, const std::vector<ByteIns>& code)
    {
        MethodValue* newMethod = new MethodValue(patternIdx, arity, nlocals, std::move(code));
        m_constants.emplace_back(newMethod);
        return m_constants.size() - 1;
    }

    void Program::print()
    {
        int i = 0;
        std::cout << "Constants pool:\n";
        for (const auto& entry : m_constants) {
            std::cout << i++ << ": ";
            entry->print();
        }
    }

    bool Program::serialize(const std::string& fileName) const
    {
        // Create the output file and open it for writing, binary
        std::ofstream bcFile(fileName, std::ios::binary);
        if (!bcFile) {
            return false;
        }
        // Serialize the constant pool
        for (const auto& entry : m_constants) {
            entry->serialize(bcFile);
        }
        return true;
    }

}