#pragma once
#include <cinttypes>
#include <unordered_map>

namespace som {

    class Frame {
    public:

        int32_t returnAddress() const { return m_returnAddress; }

    private:
        int32_t m_returnAddress;
              

    };

}