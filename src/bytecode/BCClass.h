#pragma once
#include <vector>

namespace som {

    class BCClass {
    public:

        BCClass() {}

        void registerSlot(int32_t slotIdx);
        std::vector<int32_t> slots() const { return m_slots; }

    private:
        std::vector<int32_t> m_slots;
    };

}
