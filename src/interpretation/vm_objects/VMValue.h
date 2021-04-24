#pragma once
#include <string>
#include <variant>
#include "../ProgramCounter.h"

namespace som {

    class VMObject;

    class VMValue {
    public:
        VMValue(int32_t intValue) : m_value(intValue) {}
        VMValue(double doubleValue) : m_value(doubleValue) {}
        VMValue(std::string stringValue) : m_value(stringValue) {}
        VMValue(CodeAddress address) : m_value(address) {}
        VMValue(std::vector<std::shared_ptr<VMObject>>&& array) : m_value(array) {}

        int32_t asInt() { return std::get<int32_t>(m_value); }
        double asDouble() { return std::get<double>(m_value); }
        std::string asString() { return std::get<std::string>(m_value); }
        CodeAddress asBlockAddress() { return std::get<CodeAddress>(m_value); }
        std::vector<std::shared_ptr<VMObject>>& asVector() { return std::get<std::vector<std::shared_ptr<VMObject>>>(m_value); }
        
    private:
        std::variant<int32_t, double, std::string, CodeAddress, std::vector<std::shared_ptr<VMObject>>> m_value;
    };

}