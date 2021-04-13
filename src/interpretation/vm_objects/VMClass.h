#pragma once
#include <string>
#include <vector>

#include "VMObject.h"
#include "../../bytecode/Program.h"

namespace som {

    class VMClass : public VMObject {
    public:
        VMClass() {}
        virtual ~VMClass() {}

        virtual void addSlot(const Program* program, int32_t slotIdx);

    protected:
        std::vector<std::string> m_primitives;

        virtual void addPrimitive(const Program* program, const Value* valPtr);
        void addMethod(const Program* program, const Value* valPtr);
    };

}