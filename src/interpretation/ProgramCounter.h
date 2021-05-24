#pragma once
#include <cinttypes>
#include <vector>
#include "../bytecode/Program.h"

namespace som {

    typedef std::vector<std::unique_ptr<ByteIns>>::iterator CodeAddress;
    
    class CProgramCounter {
    public:
        CProgramCounter(CodeAddress&& entryPoint, CodeAddress&& end) : m_pc(entryPoint), m_programEnd(end) {}

        void setAddress(const CodeAddress& address) { m_pc = address; }
        CodeAddress nextInstruction() { return ++m_pc; }
        CodeAddress currentInstruction() const { return m_pc; }

        bool shouldExit() const;

        CodeAddress getProgramEnd() const { return m_programEnd; }

    private:
        CodeAddress m_pc;
        CodeAddress m_programEnd;
    };

}