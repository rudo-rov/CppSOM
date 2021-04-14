#pragma once
#include <string>
#include <variant>

namespace som {

    class VMValue {
    public:
        VMValue(int32_t intValue) : m_value(intValue) {}
        VMValue(double doubleValue) : m_value(doubleValue) {}
        VMValue(std::string stringValue) : m_value(stringValue) {}

        int32_t asInt() { return std::get<int32_t>(m_value); }
        double asDouble() { return std::get<double>(m_value); }
        std::string asString() { return std::get<std::string>(m_value); }
        
    private:
        std::variant<int32_t, double, std::string> m_value;
    };

}