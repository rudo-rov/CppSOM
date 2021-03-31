#pragma once
#include <memory>
#include <queue>

#include "../bytecode/Program.h"
#include "ExecutionStack.h"

namespace som {

    class CInterpret {
    public:
        CInterpret(std::unique_ptr<Program>&& program) : m_program(std::move(program)) {}
        ~CInterpret() {}

        void interpret();

    private:
        std::unique_ptr<Program> m_program;
        CExecutionStack m_executionStack;
        // m_globalCtx;

        void objectLookup(int32_t idx);
    };

}