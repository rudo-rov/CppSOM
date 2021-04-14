#include <cassert>
#include "VMObject.h"

namespace som {

    // Should only be called on objects with primitive values - Strings, Integers, Bools etc.
    VMValue VMObject::getValue() const
    {
        assert(m_primValue);
        return m_primValue.value();
    }

}