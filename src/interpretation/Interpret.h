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

        // Getters
        std::unique_ptr<Program>& program() { return m_program; }
        CExecutionStack& executionStack() { return m_executionStack; }
        CProgramCounter& programCounter() { return m_pc; }
        CHeap& heap() { return m_heap; }
        CGlobalContext& globalContext() { return m_globalCtx; }

    private:
        std::unique_ptr<Program> m_program;
        CExecutionStack m_executionStack;
        CProgramCounter m_pc;
        CHeap m_heap;
        CGlobalContext m_globalCtx;
        
        void execute(LitIns* ins);
        void execute(SendIns* ins);
        void execute(ReturnIns* ins);
        void execute(ReturnNLIns* ins);
        void execute(SetSlotIns* ins);
        void execute(GetArgIns* ins);
        void execute(GetSelfIns* ins);
        void execute(BlockIns* ins);
        void execute(GetSlotIns* ins);

        void simpleReturn();

        std::shared_ptr<VMObject> resolveIdentifier(const std::string& identifier, std::shared_ptr<VMObject>& self);
        std::shared_ptr<VMObject> objFromValue(Value* val);
        bool shouldExit() const;
        std::shared_ptr<VMObject> newArrayObj(ArrayValue* val);
    };

}