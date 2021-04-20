#pragma once
#include <memory>
#include <queue>

#include "../bytecode/Program.h"
#include "ExecutionStack.h"
#include "ProgramCounter.h"
#include "GlobalContext.h"
#include "vm_objects/VMObject.h"
#include "Heap.h"

namespace som {

    class CInterpret {
    public:
        CInterpret(std::unique_ptr<Program>&& program) : m_program(std::move(program)), m_pc(m_program->entryPoint(), m_program->exitPoint()) {}
        ~CInterpret() = default;

        void initialize();
        void interpret();

    private:
        std::unique_ptr<Program> m_program;
        CExecutionStack m_executionStack;
        CProgramCounter m_pc;
        CHeap m_heap;
        CGlobalContext m_globalCtx;
        
        void execute(LitIns* ins);
        void execute(SendIns* ins);
        void execute(ReturnIns* ins);
        void execute(SetSlotIns* ins);
        void execute(GetArgIns* ins);
        void execute(GetSelfIns* ins);
        void execute(BlockIns* ins);
        void execute(GetSlotIns* ins);

        std::shared_ptr<VMObject> objFromValue(Value* val);
        bool shouldExit() const;
    };

}