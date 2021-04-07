#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include "Program.h"
#include "Bytecode.h"

namespace som {
        
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

    size_t Program::registerMethod(int32_t patternIdx, int32_t arity, int32_t nlocals, insVector* code)
    {
        MethodValue* newMethod = new MethodValue(patternIdx, arity, nlocals, code);
        m_constants.emplace_back(newMethod);
        return m_constants.size() - 1;
    }

    int32_t Program::registerMethod(int32_t patternIdx)
    {
        PrimitiveValue* newMethod = new PrimitiveValue(patternIdx);
        m_constants.emplace_back(newMethod);
        return m_constants.size() - 1;
    }

    int32_t Program::registerClass(int32_t identifierIdx, std::vector<int32_t>& slots)
    {
        m_constants.emplace_back(new ClassValue(identifierIdx, slots));
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
        std::cout << "------------\n";
        std::cout << "Entry point: " << m_entryPoint << std::endl;

    }

    bool Program::setEntryPoint(int32_t entryPoint)
    {
        // If entry point is not set to default-1, there are multiple entry points = error
        if (m_entryPoint == -1) {
            m_entryPoint = entryPoint;
            return true;
        }
        return false;
    }

    Value* Program::getValue(int32_t idx) const
    {
        return m_constants.at(idx).get();
    }

    bool Program::serialize(const std::string& fileName) const
    {
        // Create the output file and open it for writing, binary
        std::ofstream bcFile(fileName, std::ios::binary);
        if (!bcFile) {
            return false;
        }
        bcFile.write(reinterpret_cast<const char*>(&m_entryPoint), sizeof m_entryPoint);
        // Serialize the constant pool
        for (const auto& entry : m_constants) {
            entry->serialize(bcFile);
        }
        return true;
    }

    int32_t Program::indexOf(const std::string& value)
    {
        for (auto i = 0; i < m_constants.size(); ++i) {
            if (m_constants.at(i)->tag == ValueTag::StringVal) {
                StringValue* strVal = static_cast<StringValue*>(m_constants.at(i).get());
                if (strVal->value == value) {
                    return i;
                }
            }
        }
        return registerConstant(value); 
    }

    int32_t Program::indexOf(int32_t value)
    {
        for (auto i = 0; i < m_constants.size(); ++i) {
            if (m_constants.at(i)->tag == ValueTag::IntVal) {
                IntValue* intVal = dynamic_cast<IntValue*>(m_constants.at(i).get());
                if (intVal->value == value) {
                    return i;
                }
            }
        }
        return registerConstant(value);
    }

    int32_t Program::indexOf(double value)
    {
        for (auto i = 0; i < m_constants.size(); ++i) {
            if (m_constants.at(i)->tag == ValueTag::DoubleVal) {
                DoubleValue* doubleVal = dynamic_cast<DoubleValue*>(m_constants.at(i).get());
                if (doubleVal->value == value) {
                    return i;
                }
            }
        }
        return registerConstant(value);
    }

    std::vector<std::unique_ptr<ByteIns>>::iterator Program::entryPoint()
    {
        MethodValue* runMethod = dynamic_cast<MethodValue*>(m_constants.at(m_entryPoint).get());
        return runMethod->code->begin();
    }

    std::vector<std::unique_ptr<ByteIns>>::iterator Program::exitPoint()
    {
        MethodValue* runMethod = dynamic_cast<MethodValue*>(m_constants.at(m_entryPoint).get());
        return runMethod->code->end();
    }
}