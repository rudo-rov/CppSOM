#include "ObjectReference.h"

namespace som {

    /* static */ CObjectReference* CObjectReference::fromValue(Value* val)
    {
        switch (val->tag)
        {
        case ValueTag::StringVal: {
            StringValue* strVal = static_cast<StringValue*>(val);
            return new CStringObjectReference(strVal->value);
        }
            
        
        default:
            return new CObjectReference();
        }
    }

}