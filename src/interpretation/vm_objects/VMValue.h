#pragma once
#include <string>
#include <variant>
#include <vector>
#include "../ProgramCounter.h"

namespace som {

    class VMObject;
    class Frame;

    struct BlockContext {
        BlockContext(CodeAddress addr, std::shared_ptr<Frame>& homeCtx) : address(addr), homeCtx(homeCtx) {}
        CodeAddress address;
        std::shared_ptr<Frame> homeCtx;
    };

    class VMValue {
    public:
        VMValue(int32_t intValue) : m_value(intValue) {}
        VMValue(double doubleValue) : m_value(doubleValue) {}
        VMValue(std::string stringValue) : m_value(stringValue) {}
        VMValue(CodeAddress address, std::shared_ptr<Frame>& homeCtx) : m_value(BlockContext(address, homeCtx)) {}
        VMValue(std::vector<std::shared_ptr<VMObject>>&& array) : m_value(array) {}

        int32_t asInt() { return std::get<int32_t>(m_value); }
        double asDouble() { return std::get<double>(m_value); }
        BlockContext& asBlockContext() { return std::get<BlockContext>(m_value); }
        std::string asString() { return std::get<std::string>(m_value); }
        std::vector<std::shared_ptr<VMObject>>& asVector() { return std::get<std::vector<std::shared_ptr<VMObject>>>(m_value); }
        
    private:
        std::variant<int32_t, double, std::string, BlockContext, std::vector<std::shared_ptr<VMObject>>> m_value;
    };

}