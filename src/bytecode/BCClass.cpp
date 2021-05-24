#include "BCClass.h"

namespace som {

    void BCClass::registerSlot(int32_t slotIdx)
    {
        m_slots.push_back(slotIdx);
    }

}